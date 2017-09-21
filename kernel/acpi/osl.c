#include <_/vineyard.h>
#include <acpi/acpi.h>
#include <cpu/ports.h>
#include <mm/alloc.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <lock/mutex.h>
#include <lock/semaphore.h>
#include <time/pit.h>
#include <proc/thread.h>

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"

ACPI_OSD_HANDLER sci_handler;
void *sci_context;

ACPI_STATUS AcpiDbSingleStep(void *state __unused, void *op __unused, uint32_t optype __unused);
void AcpiDbSignalBreakPoint(void *state __unused);
void AcpiDbDumpMethodInfo(ACPI_STATUS status __unused, void *state __unused);
void AcpiDbDisplayArgumentObject(void *desc __unused, void *state __unused);
void AcpiDbDisplayResultObject(void *desc __unused, void *walk __unused);

static void acpi_sci_handler(frame_t *frame __unused) {
	sci_handler(sci_context);
}

/* 9.1 Environmental and ACPI Tables */
ACPI_STATUS AcpiOsInitialize(void) {
	return AE_OK;
}

ACPI_STATUS AcpiOsTerminate(void) {
	return AE_OK;
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer(void) {
	ACPI_PHYSICAL_ADDRESS ret = 0;

	AcpiFindRootPointer(&ret);

	return ret;
}

ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES *obj __unused, ACPI_STRING *new) {
	*new = NULL;

	return AE_OK;
}

ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER *existing __unused, ACPI_TABLE_HEADER **new) {
	*new = NULL;

	return AE_OK;
}

ACPI_STATUS AcpiOsPhysicalTableOverride(ACPI_TABLE_HEADER *existing __unused, ACPI_PHYSICAL_ADDRESS *new, uint32_t *len __unused) {
	*new = 0;

	return AE_OK;
}

/* 9.2 Memory Management */

/* AcpiOsCreateCache is disabled */
/* AcpiOsDeleteCache is disabled */
/* AcpiOsPurgeCache is disabled */
/* AcpiOsAcquireObject is disabled */
/* AcpiOsReleaseObject is disabled */

void *AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS addr, ACPI_SIZE length) {
	length = ALIGN_UP(length, 0x1000);
	void *virt = mm_alloc(length, PAGE_PRESENT | PAGE_WRITE, false);

	uintptr_t v = (uintptr_t) virt;
	uintptr_t p = addr & 0xFFFFF000;

	for(; v - (uintptr_t) virt < length; v += 0x1000, p += 0x1000) {
		mm_physical_mark_reserved(p);
		mm_virtual_map_page(v, p, PAGE_PRESENT | PAGE_WRITE);
	}

	return (void *) ((uintptr_t) virt + (uintptr_t) (addr & 0xFFF));
}

void AcpiOsUnmapMemory(void *ptr, ACPI_SIZE length) {
	uintptr_t addr = (uintptr_t) ptr & 0xFFFFF000;
	mm_alloc_free((void *) addr);

	uintptr_t p = (uintptr_t) ptr;

	for(; p - (uintptr_t) ptr < length; p += 0x1000) {
		mm_physical_mark_free(p);
	}
}

ACPI_STATUS AcpiOsGetPhysicalAddress(void *logical, ACPI_PHYSICAL_ADDRESS *ptr) {
	if(!logical || !ptr) {
		return AE_BAD_PARAMETER;
	}

	uintptr_t phys = mm_virtual_get_physical((uintptr_t) logical);

	if(!phys) {
		return AE_ERROR;
	}

	*ptr = phys;

	return AE_OK;
}

void *AcpiOsAllocate(ACPI_SIZE size) {
	return malloc(size);
}

void AcpiOsFree(void *ptr) {
	free(ptr);
}

BOOLEAN AcpiOsReadable(void *memory __unused, ACPI_SIZE length __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);
}

BOOLEAN AcpiOsWritable(void *memory __unused, ACPI_SIZE length __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);
}

/* 9.3 Multithreading and Scheduling Services */
ACPI_THREAD_ID AcpiOsGetThreadId(void) {
	thread_t *t = thread_get();

	return t->tid + 1;
}

ACPI_STATUS AcpiOsExecute(ACPI_EXECUTE_TYPE type __unused, ACPI_OSD_EXEC_CALLBACK func, void *context) {
	if(!func) {
		return AE_BAD_PARAMETER;
	}

	proc_create_worker(func, context);

	return AE_OK;
}

void AcpiOsSleep(UINT64 milliseconds __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);
}

void AcpiOsStall(uint32_t microseconds __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);
}

void AcpiOsWaitEventsComplete(void) {
	/* TODO: figure out what this is supposed to do exactly */
}

/* 9.4 Mutual Exclusion and Synchronization */
ACPI_STATUS AcpiOsCreateMutex(ACPI_MUTEX *mutex) {
	mutex_t *m = mutex_create();

	if(!m) {
		return AE_NO_MEMORY;
	}

	*mutex = m;

	return AE_OK;
}

void AcpiOsDeleteMutex(ACPI_MUTEX mutex) {
	if(mutex) {
		mutex_remove(mutex);
	}
}

ACPI_STATUS AcpiOsAcquireMutex(ACPI_MUTEX mutex, uint16_t timeout) {
	assert(timeout == 0xFFFF);

	mutex_acquire(mutex);

	return 0;
}

void AcpiOsReleaseMutex(ACPI_MUTEX mutex) {
	mutex_release(mutex);
}

ACPI_STATUS AcpiOsCreateSemaphore(uint32_t max_units __unused, uint32_t initial_units, ACPI_SEMAPHORE *s) {
	assert(initial_units <= INT_MAX);

	semaphore_t *m = semaphore_create_init((int) initial_units);

	if(!m) {
		return AE_NO_MEMORY;
	}

	*s = m;

	return AE_OK;

	return AE_OK;
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE s) {
	if(!s) {
		return AE_BAD_PARAMETER;
	}

	semaphore_remove(s);

	return AE_OK;
}

ACPI_STATUS AcpiOsWaitSemaphore(ACPI_SEMAPHORE s, uint32_t units, uint16_t timeout) {
	assert(timeout == 0xFFFF);
	assert(units <= INT_MAX);

	semaphore_wait(s, (int) units);

	return AE_OK;
}

ACPI_STATUS AcpiOsSignalSemaphore(ACPI_SEMAPHORE s, uint32_t units) {
	assert(units <= INT_MAX);

	semaphore_signal(s, (int) units);

	return AE_OK;
}

ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK *lock) {
	spinlock_t *s = malloc(sizeof(*s));

	if(!s) {
		return AE_NO_MEMORY;
	}

	*lock = s;

	return AE_OK;
}

void AcpiOsDeleteLock(ACPI_SPINLOCK lock) {
	free(lock);
}

ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK lock) {
	spinlock_acquire(lock);

	return AE_OK;
}

void AcpiOsReleaseLock(ACPI_SPINLOCK lock, ACPI_CPU_FLAGS flags __unused) {
	spinlock_release(lock);
}

/* 9.5 interrupt Handling */
ACPI_STATUS AcpiOsInstallInterruptHandler(uint32_t interrupt, ACPI_OSD_HANDLER hand, void *context) {
	if(interrupt > 0xFF || !hand) {
		return AE_BAD_PARAMETER;
	} else if(handler_get(interrupt)) {
		return AE_ALREADY_EXISTS;
	}

	handler_set(interrupt, acpi_sci_handler);

	sci_handler = hand;
	sci_context = context;

	return AE_OK;
}

ACPI_STATUS AcpiOsRemoveInterruptHandler(uint32_t interrupt, ACPI_OSD_HANDLER hand) {
	if(interrupt > 0xFF || !hand || hand != (ACPI_OSD_HANDLER) handler_get(interrupt)) {
		return AE_BAD_PARAMETER;
	} else if(!handler_get(interrupt)) {
		return AE_NOT_EXIST;
	}

	handler_delete(interrupt);

	return AE_OK;
}

/* 9.6 Memory Access */
ACPI_STATUS AcpiOsReadMemory(ACPI_PHYSICAL_ADDRESS addr, uint64_t *value, uint32_t width) {
	void *ptr;

	if(addr < 0x100000) {
		ptr = (void *) (0xC0000000 | (uintptr_t) addr);
	} else {
		ptr = mm_alloc(0x1000, PAGE_PRESENT | PAGE_WRITE, false);
		mm_virtual_map_page((uintptr_t) ptr, addr & 0xFFFFF000, PAGE_PRESENT | PAGE_WRITE);
	}

	switch(width) {
		case 8: *value = *(uint8_t *) ptr; break;
		case 16: *value = *(uint16_t *) ptr; break;
		case 32: *value = *(uint32_t *) ptr; break;
		case 64: *value = *(uint64_t *) ptr; break;
		default: return AE_BAD_PARAMETER;
	}

	if(addr >= 0x100000) {
		mm_alloc_free(ptr);
	}

	return AE_OK;
}

ACPI_STATUS AcpiOsWriteMemory(ACPI_PHYSICAL_ADDRESS addr, uint64_t value, uint32_t width) {
	void *ptr;

	if(addr < 0x100000) {
		ptr = (void *) (0xC0000000 | (uintptr_t) addr);
	} else {
		ptr = mm_alloc(0x1000, PAGE_PRESENT | PAGE_WRITE, false);
		mm_virtual_map_page((uintptr_t) ptr, addr & 0xFFFFF000, PAGE_PRESENT | PAGE_WRITE);
		ptr = (void *) ((uintptr_t) ptr + (uintptr_t) (addr & 0xFFF));
	}

	switch(width) {
		case 8: *(uint8_t *) ptr = (uint8_t) value; break;
		case 16: *(uint16_t *) ptr = (uint16_t) value; break;
		case 32: *(uint32_t *) ptr = (uint32_t) value; break;
		case 64: *(uint64_t *) ptr = (uint64_t) value; break;
		default: return AE_BAD_PARAMETER;
	}

	if(addr >= 0x100000) {
		mm_alloc_free((void *) ((uintptr_t) ptr & 0xFFFFF000));
	}

	return AE_OK;
}

/* 9.7 Port Input/Output */
ACPI_STATUS AcpiOsReadPort(ACPI_IO_ADDRESS addr, uint32_t *value, uint32_t width) {
	uint16_t a = (uint16_t) addr;

	switch(width) {
		case 8: {
			*value = inb(a);
			break;
		}
		case 16: {
			*value = inw(a);
			break;
		}
		case 32: {
			*value = inl(a);
			break;
		}
		default: {
			return AE_BAD_PARAMETER;
		}
	}

	return AE_OK;
}

ACPI_STATUS AcpiOsWritePort(ACPI_IO_ADDRESS addr, uint32_t value, uint32_t width) {
	uint16_t a = (uint16_t) addr;

	switch(width) {
		case 8: {
			outb(a, (uint8_t) value);
			break;
		}
		case 16: {
			outw(a, (uint16_t) value);
			break;
		}
		case 32: {
			outl(a, value);
			break;
		}
		default: {
			return AE_BAD_PARAMETER;
		}
	}

	return AE_OK;
}

/* 9.8 PIC Configuration Space Access */
ACPI_STATUS AcpiOsReadPciConfiguration(ACPI_PCI_ID *id __unused, uint32_t reg __unused, UINT64 *val __unused, uint32_t width __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);

	return AE_OK;
}

ACPI_STATUS AcpiOsWritePciConfiguration(ACPI_PCI_ID *id __unused, uint32_t reg __unused, UINT64 val __unused, uint32_t width __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);

	return AE_OK;
}

/* 9.9 Formatted Output */
void AcpiOsPrintf(const char *format, ...) {
	va_list args;
	va_start(args, format);

	vprintf(format, args);

	va_end(args);
}

void AcpiOsVprintf(const char *format, va_list args) {
	vprintf(format, args);
}

void AcpiOsRedirectOutput(void *destination __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);
}

/* 9.10 System ACPI Table Access */
ACPI_STATUS AcpiOsGetTableByAddress(ACPI_PHYSICAL_ADDRESS address __unused, ACPI_TABLE_HEADER **out __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);

	return AE_SUPPORT;
}

ACPI_STATUS AcpiOsGetTableByIndex(uint32_t index __unused, ACPI_TABLE_HEADER **table __unused, uint32_t *instance __unused, ACPI_PHYSICAL_ADDRESS *address __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);

	return AE_SUPPORT;
}

ACPI_STATUS AcpiOsGetTableByName(char *signature __unused, uint32_t instance __unused, ACPI_TABLE_HEADER **table __unused, ACPI_PHYSICAL_ADDRESS *address __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);

	return AE_SUPPORT;
}

/* 9.11 Miscellaneous */
UINT64 AcpiOsGetTimer(void) {
	return pit_uptime() * 10000;
}

ACPI_STATUS AcpiOsSignal(uint32_t function, void *info __unused) {
	switch(function) {
		case ACPI_SIGNAL_FATAL: {
			panic("[acpi]	AML fatal opcode");
			break;
		}
		case ACPI_SIGNAL_BREAKPOINT: {
			printf("[acpi]	AML breakpoint");
			break;
		}
		default: {
			return AE_BAD_PARAMETER;
		}
	}

	return AE_OK;
}

ACPI_STATUS AcpiOsGetLine(char *buffer __unused, uint32_t buffer_length __unused, uint32_t *bytes_read __unused) {
	panic("[acpi]	%s unimplemented", __FUNCTION__);
}

/* TODO: allow for handlers to register? */
ACPI_STATUS AcpiOsEnterSleep(uint8_t state __unused, uint32_t a __unused, uint32_t b __unused) {
	return AE_OK;
}

/* Debugging stubs */
ACPI_STATUS AcpiDbSingleStep(void *state __unused, void *op __unused, uint32_t optype __unused) {
       panic("[acpi]   %s unimplemented", __FUNCTION__);

       return AE_OK;
}

void AcpiDbSignalBreakPoint(void *state __unused) {
       panic("[acpi]   %s unimplemented", __FUNCTION__);
}

void AcpiDbDumpMethodInfo(ACPI_STATUS status __unused, void *state __unused) {
       panic("[acpi]   %s unimplemented", __FUNCTION__);
}

void AcpiDbDisplayArgumentObject(void *desc __unused, void *state __unused) {
       panic("[acpi]   %s unimplemented", __FUNCTION__);
}

void AcpiDbDisplayResultObject(void *desc __unused, void *walk __unused) {
       panic("[acpi]   %s unimplemented", __FUNCTION__);
}
