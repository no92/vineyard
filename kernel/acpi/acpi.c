#include <assert.h>
#include <stdio.h>

#include <acpi/acpi.h>

void acpi_init(void) {
	ACPI_STATUS s;

	s = AcpiInitializeSubsystem();
	assert(!ACPI_FAILURE(s));

	s = AcpiInitializeTables(NULL, 16, false);
	if(ACPI_FAILURE(s)) {
		AcpiTerminate();
		assert(!ACPI_FAILURE(s));
	}

	s = AcpiLoadTables();
	if(ACPI_FAILURE(s)) {
		AcpiTerminate();
		assert(!ACPI_FAILURE(s));
	}

	s = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
	if(ACPI_FAILURE(s)) {
		AcpiTerminate();
		assert(!ACPI_FAILURE(s));
	}
}
