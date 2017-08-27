[bits 32]

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
[extern _init]
[extern kernel]

[global _start]
_start:
	mov ecx, (boot_pagedir - 0xC0000000)
	mov cr3, ecx
	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx
	lea ecx, [.higherHalf]
	jmp ecx

.higherHalf:
	; Clear the identity mapping from the page directory
	mov edx, boot_pagedir
	xor ecx, ecx
	mov [edx], ecx
	invlpg [0]

	; set up a stack
	mov esp, stack_top
	mov ebp, stack_top

	; set up the arguments for the kernel
	add ebx, 0xC0000000
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

[section .data]
[align 0x1000]
boot_pagedir:
	dd (boot_pagetab - 0xC0000000) + 0x03
	times ((0xC0000000 >> 22) - 1) dd 0x00
	dd (boot_pagetab - 0xC0000000) + 0x03
	times (1022 - (0xC0000000 >> 22)) dd 0x00
	dd (boot_pagedir - 0xC0000000) + 0x03
boot_pagetab:
	%assign i 0
	%rep 1024
		dd (i << 12) | 0x03
		%assign i i+1
	%endrep
