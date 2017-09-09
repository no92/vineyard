[section .text]

[global _start]
[extern _init]
[extern main]
[extern exit]
_start:
	mov ebp, 0
	push ebp
	push ebp
	mov ebp, esp

	call _init
	call main
	push eax
	call exit
