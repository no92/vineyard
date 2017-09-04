[global mm_virtual_switch]
mm_virtual_switch:
	push ebp
	mov ebp, esp
	mov eax, [esp + 8]
	mov cr3, eax
	pop ebp
	ret
