format ELF64

section '.text'

;; rdi: new rsp
public ctx_goto
ctx_goto:
	mov rsp, rdi

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	pop rbx
	pop rdi
	ret
