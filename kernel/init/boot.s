[bits 32]

; this magic value is required by the spec to be the first dword in the header
MB2_MAGIC equ 0xE85250D6

; tag type definitions
MB2_TAG_TERMINATE equ 0
MB2_TAG_FRAMEBUFFER equ 5

; this is the offset we add to the kernel's physical address (1 MiB)
; its value is 3 GiB (= 0xC0000000), but in order to prevent warnings we use the corresponding signed value
KERNEL_VIRTUAL_OFFSET equ -0x40000000
; for convenience, we define KERNEL_VIRTUAL_UOFFSET as the true (unsigned value), mostly for bitshifting
KERNEL_VIRTUAL_UOFFSET equ 0xC0000000

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
	mov ecx, dword (boot_pagedir - KERNEL_VIRTUAL_OFFSET)
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
	add ebx, KERNEL_VIRTUAL_OFFSET
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
[global stack_top]
stack_bottom:
	resb 16384
stack_top:

[section .data]
[align 0x1000]
boot_pagedir:
	dd (boot_pagetab_low - KERNEL_VIRTUAL_OFFSET) + 0x03
	dd (boot_pagetab_high - KERNEL_VIRTUAL_OFFSET) + 0x03
	times ((KERNEL_VIRTUAL_UOFFSET >> 22) - 2) dd 0x00
	dd (boot_pagetab_low - KERNEL_VIRTUAL_OFFSET) + 0x03
	dd (boot_pagetab_high - KERNEL_VIRTUAL_OFFSET) + 0x03
	times (1021 - (KERNEL_VIRTUAL_UOFFSET >> 22)) dd 0x00
	dd (boot_pagedir - KERNEL_VIRTUAL_OFFSET) + 0x03
boot_pagetab_low:
	%assign i 0
	%rep 1024
		dd (i << 12) | 0x03
		%assign i i+1
	%endrep
boot_pagetab_high:
	%assign i 1024
	%rep 1024
		dd (i << 12) | 0x03
		%assign i i+1
	%endrep
