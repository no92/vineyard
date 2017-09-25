#ifndef __ACVINEYARD_H__
#define __ACVINEYARD_H__

#include <lock/spinlock.h>

#include <ctype.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#define ACPI_SINGLE_THREADED
#define ACPI_DEBUG_OUTPUT
#define ACPI_USE_SYSTEM_CLIBRARY

#define ACPI_CACHE_T                ACPI_MEMORY_LIST
#define ACPI_USE_LOCAL_CACHE        1

#define ACPI_USE_DO_WHILE_0
#define ACPI_MUTEX_TYPE             ACPI_OSL_MUTEX
#define ACPI_SPINLOCK               spinlock_t *
#define ACPI_USE_NATIVE_DIVIDE

#define ACPI_MACHINE_WIDTH          32

#define ACPI_MUTEX                  void *

#define ACPI_UINTPTR_T              uintptr_t

#include "acgcc.h"

#endif
