%macro irq 1
[global isr_handle_irq%1]
isr_handle_irq%1:
	push 0
	push %1+32
	jmp isr_dispatch_setup
%endmacro

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15
irq 16
irq 17
irq 18
irq 19
irq 20
irq 21
irq 22
irq 23

[global isr_handle_0]
isr_handle_0:
	push 0
	push 0
	jmp isr_dispatch_setup

[global isr_handle_1]
isr_handle_1:
	push 0
	push 1
	jmp isr_dispatch_setup

[global isr_handle_2]
isr_handle_2:
	push 0
	push 2
	jmp isr_dispatch_setup

[global isr_handle_3]
isr_handle_3:
	push 0
	push 3
	jmp isr_dispatch_setup

[global isr_handle_4]
isr_handle_4:
	push 0
	push 4
	jmp isr_dispatch_setup

[global isr_handle_5]
isr_handle_5:
	push 0
	push 5
	jmp isr_dispatch_setup

[global isr_handle_6]
isr_handle_6:
	push 0
	push 6
	jmp isr_dispatch_setup

[global isr_handle_7]
isr_handle_7:
	push 0
	push 7
	jmp isr_dispatch_setup

[global isr_handle_8]
isr_handle_8:
	push 8
	jmp isr_dispatch_setup

[global isr_handle_9]
isr_handle_9:
	push 0
	push 9
	jmp isr_dispatch_setup

[global isr_handle_10]
isr_handle_10:
	push 10
	jmp isr_dispatch_setup

[global isr_handle_11]
isr_handle_11:
	push 11
	jmp isr_dispatch_setup

[global isr_handle_12]
isr_handle_12:
	push 12
	jmp isr_dispatch_setup

[global isr_handle_13]
isr_handle_13:
	push 13
	jmp isr_dispatch_setup

[global isr_handle_14]
isr_handle_14:
	push 14
	jmp isr_dispatch_setup

[global isr_handle_15]
isr_handle_15:
	push 0
	push 15
	jmp isr_dispatch_setup

[global isr_handle_16]
isr_handle_16:
	push 0
	push 16
	jmp isr_dispatch_setup

[global isr_handle_17]
isr_handle_17:
	push 17
	jmp isr_dispatch_setup

[global isr_handle_18]
isr_handle_18:
	push 0
	push 18
	jmp isr_dispatch_setup

[global isr_handle_19]
isr_handle_19:
	push 0
	push 19
	jmp isr_dispatch_setup

[global isr_handle_20]
isr_handle_20:
	push 0
	push 20
	jmp isr_dispatch_setup

[global isr_handle_21]
isr_handle_21:
	push 0
	push 21
	jmp isr_dispatch_setup

[global isr_handle_22]
isr_handle_22:
	push 0
	push 22
	jmp isr_dispatch_setup

[global isr_handle_23]
isr_handle_23:
	push 0
	push 23
	jmp isr_dispatch_setup

[global isr_handle_24]
isr_handle_24:
	push 0
	push 24
	jmp isr_dispatch_setup

[global isr_handle_25]
isr_handle_25:
	push 0
	push 25
	jmp isr_dispatch_setup

[global isr_handle_26]
isr_handle_26:
	push 0
	push 26
	jmp isr_dispatch_setup

[global isr_handle_27]
isr_handle_27:
	push 0
	push 27
	jmp isr_dispatch_setup

[global isr_handle_28]
isr_handle_28:
	push 0
	push 28
	jmp isr_dispatch_setup

[global isr_handle_29]
isr_handle_29:
	push 0
	push 29
	jmp isr_dispatch_setup

[global isr_handle_30]
isr_handle_30:
	push 0
	push 30
	jmp isr_dispatch_setup

[global isr_handle_31]
isr_handle_31:
	push 0
	push 31
	jmp isr_dispatch_setup

[global isr_handle_lvt_timer]
isr_handle_lvt_timer:
	push 0
	push 0xFD
	jmp isr_dispatch_setup

[global isr_handle_lvt_error]
isr_handle_lvt_error:
	push 0
	push 0xFE
	jmp isr_dispatch_setup

[global isr_handle_spurious]
isr_handle_spurious:
	push 0
	push 0xFF
	jmp isr_dispatch_setup

[extern isr_dispatch]
[global isr_dispatch_setup]
isr_dispatch_setup:
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax

	cld
	mov rdi, rsp
	call isr_dispatch

	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	add rsp, 16
	iretq
