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

[extern handler]
handler_setup:
	pusha

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp
	cld
	call handler
	add esp, 4

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	mov ss, bx

	popa
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
