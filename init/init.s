[section .text]
[global _start]
_start:
	mov eax, 0x01
	mov ebx, hello_world
	int 0x80
	mov eax, 0x02
	mov ebx, 0x00
	int 0x80
	jmp $

[section .data]
hello_world:
db "Hello World", 0x0A, 0x00
