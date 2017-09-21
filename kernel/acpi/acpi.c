#include <assert.h>

#include <acpi/acpi.h>

void acpi_init(void) {
	ACPI_STATUS s;

	s = AcpiInitializeSubsystem();
	assert(!ACPI_FAILURE(s));

	s = AcpiInitializeTables(NULL, 16, false);
	assert(!ACPI_FAILURE(s));

	s = AcpiLoadTables();
	assert(!ACPI_FAILURE(s));

	s = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
	assert(!ACPI_FAILURE(s));
}
