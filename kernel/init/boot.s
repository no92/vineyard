; this magic value is required by the spec to be the first dword in the header
MB2_MAGIC equ 0xE85250D6

; tag type definitions
MB2_TAG_TERMINATE equ 0
MB2_TAG_FRAMEBUFFER equ 5

; we put the header in its own section to make sure it is located in the first 32 KiB of the binary
[section .multiboot2 align=8]
mb2_header_start:
	dd MB2_MAGIC
	dd 0
	dd mb2_header_end - mb2_header_start
	dd (0 - MB2_MAGIC - (mb2_header_end - mb2_header_start)) & 0xFFFFFFFF

	[align 8]
	tag_framebuffer:
		dw MB2_TAG_FRAMEBUFFER
		dw 0
		dd (tag_framebuffer_end - tag_framebuffer)
		dd MB2_WIDTH
		dd MB2_HEIGHT
		dd 32
	tag_framebuffer_end:

	[align 8]
	tag_terminator_start:
		dw MB2_TAG_TERMINATE
		dw 0
		dd (tag_terminator_end - tag_terminator_start)
	tag_terminator_end:
mb2_header_end:

[section .text]
[global _start]
[extern kernel]
[extern _init]

_start:
	; set up a stack
	mov esp, stack_top

	; set up the arguments for the kernel
	push ebx ; holds the physical address of the multiboot struct
	push eax ; holds the magic number

	call _init

	call kernel

	; loop forever
	cli
	hlt
	jmp $

[section .bss]
[align 16]
stack_bottom:
	resb 16384
stack_top:
