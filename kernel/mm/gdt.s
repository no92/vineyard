[extern gdtr]
[global gdt_reload]
gdt_reload:
	lgdt [gdtr]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.reload
.reload:
	ret
