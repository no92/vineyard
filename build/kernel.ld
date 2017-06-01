ENTRY(_start)

SECTIONS {
	. = 1M;

	_kernel_start = .;

	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot2)
		_text_start = .;
		*(.text)
		_text_end = .;
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K) {
		*(.bss)
	}

	_kernel_end = .;
}
