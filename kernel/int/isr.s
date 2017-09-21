[extern current]

%macro EXCEPTION 1
	[global exception%1]
	exception%1:
		cli
		push 0
		push %1
		jmp handler_setup
%endmacro

%macro EXCEPTIONE 1
	[global exception%1]
	exception%1:
		cli
		push %1
		jmp handler_setup
%endmacro

%macro IRQ 1
	[global irq%1]
	irq%1:
		cli
		push 0
		push %1 + 32
		jmp handler_setup
%endmacro

[global handle_syscall]
handle_syscall:
	cli
	push 0
	push 0x80
	jmp handler_setup

[extern handler]
[global handler_setup]
handler_setup:
	test word [esp + 12], 0x03 ; set flags according to the value of (cs & 0x03)
	jz handler_stack_relocate ; jump if we're coming from ring 0

.do:
	; push segment registers
	push ds
	push es
	push fs
	push gs

	pusha ; push all general-purpose registers, including (a useless and discarded) esp

	; fill the segment registers with the value for the kernel data segment
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; push the interrupt frame onto the stack as argument
	push esp
	cld
	call handler ; handler(frame)
	add esp, 4 ; clean up argument

	popa ; pop all general-purpose registers

	; pop all segment registers
	pop gs
	pop fs
	pop es
	pop ds

	; discard interrupt and error numbers
	add esp, 8

	; was this interrupt fired when CPL=0
	test word [esp + 4], 0x03
	; if yes, branch
	jz handler_stack_restore
.user:
	; else, return
	iret

; handler_stack_relocate and handler_stack_restore are derived from sortix
; Copyright 2011-2016 Jonas 'Sortie' Termansen and contributors.
; Sortix is free software licensed under the ISC license; see README.md for details
handler_stack_relocate:
	mov [esp - 4 - 8], eax	; save eax onto the stack
	mov eax, [esp + 0]		; move frame->interrupt into eax
	mov [esp + 0 - 8], eax	; insert frame->interrupt 8 bytes higher on the stack
	mov eax, [esp + 4]		; move frame->error into eax
	mov [esp + 4 - 8], eax	; insert frame->error 8 bytes higher on the stack
	mov eax, [esp + 8]		; move frame->eip into eax
	mov [esp + 8 - 8], eax	; insert frame->eip 8 bytes higher on the stack
	mov eax, [esp + 12]		; move frame->cs into eax
	mov [esp + 12 - 8], eax	; insert frame->cs 8 bytes higher on the stack
	mov eax, [esp + 16]		; move frame->eflags into eax
	mov [esp + 16 - 8], eax	; insert frame->eflags 8 bytes higher on the stack

	mov eax, esp			; load esp into eax
	add eax, 20				; eax now points at stack top (frame->ds)
	mov [esp + 20 - 8], eax	; save eax as frame->esp
	mov eax, ss				; load ss into eax
	mov [esp + 24 - 8], eax	; save eax as frame->ss

	mov eax, [esp - 4 - 8]	; restore eax from the stack
	sub esp, 8				; adjust stack top to accomodate frame->ss and frame->esp
	jmp handler_setup.do

handler_stack_restore:
	mov [esp - 4], eax		; save frame->eax to the stack
	mov [esp - 8], ebx		; save frame->ebx to the stack
	mov [esp - 12], ecx		; save frame->ecx to the stack

	mov ebx, [esp + 12]		; load frame->esp into ebx
	sub ebx, 12				; let it point to eip
	mov cx, word [esp + 16]	; load frame->ss

	mov eax, [esp - 4]		; move frame->eax into eax
	mov [ebx - 12], eax		; insert frame->eax 8 bytes higher than frame->eip
	mov eax, [esp - 8]		; move frame->ebx into eax
	mov [ebx - 16], eax		; insert frame->ebx 8 bytes higher than frame->eip
	mov eax, [esp - 12]		; move frame->ecx into eax
	mov [ebx - 20], eax		; insert frame->ecx 8 bytes higher than frame->eip

	mov eax, [esp + 8]		; load eflags from old stack
	mov [ebx + 8], eax		; and store it on the new stack
	mov eax, [esp + 4]		; load cs from old stack
	mov [ebx + 4], eax		; and store it on the new stack
	mov eax, [esp + 0]		; load eip from old stack
	mov [ebx + 0], eax		; and store it on the new stack

	mov ss, cx				; store frame->ss in ss
	mov esp, ebx			; store frame->esp in esp

	mov eax, [esp - 12]		; restore frame->eax from the stack
	mov ebx, [esp - 16]		; restore frame->ebx from the stack
	mov ecx, [esp - 20]		; restore frame->ecx from the stack
	jmp handler_setup.user

EXCEPTION 0
EXCEPTION 1
EXCEPTION 2
EXCEPTION 3
EXCEPTION 4
EXCEPTION 5
EXCEPTION 6
EXCEPTION 7
EXCEPTIONE 8
EXCEPTION 9
EXCEPTIONE 10
EXCEPTIONE 11
EXCEPTIONE 12
EXCEPTIONE 13
EXCEPTIONE 14
EXCEPTION 15
EXCEPTION 16
EXCEPTIONE 17
EXCEPTION 18
EXCEPTION 19
EXCEPTIONE 20
EXCEPTION 21
EXCEPTION 22
EXCEPTION 23
EXCEPTION 24
EXCEPTION 25
EXCEPTION 26
EXCEPTION 27
EXCEPTION 28
EXCEPTION 29
EXCEPTION 30 ; osdev.org says it pushes an error, while the manual reserves this interrupt
EXCEPTION 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
