%macro EXCEPTION 1
	[global exception%1]
	exception%1:
		push 0
		push %1
		jmp handler_setup
%endmacro

%macro EXCEPTIONE 1
	[global exception%1]
	exception%1:
		push %1
		jmp handler_setup
%endmacro

%macro IRQ 1
	[global irq%1]
	irq%1:
		push 0
		push %1 + 32
		jmp handler_setup
%endmacro

[global handle_syscall]
handle_syscall:
	push 0
	push 0x80
	jmp handler_setup

[extern handler]
[global handler_setup]
handler_setup:
	cli
	push ds
	push es
	push fs
	push gs

	pusha

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	push esp
	cld
	call handler
	add esp, 4

	popa

	pop gs
	pop fs
	pop es
	pop ds

	add esp, 8
	iret

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
