[section .text]

[global _start]
[extern _init]
[extern main]
[extern exit]
_start:
	mov ebp, 0
	mov ebp, esp

	pop eax
	pop eax
	xor eax, eax

	call _init
	call main
	push eax
	call exit
