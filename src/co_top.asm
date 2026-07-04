format ELF64

section '.text'

;; rdi: stack size
public co_top
co_top:
	mov rax, rsp
	mov rcx, rdi
	sub rcx, 1
	not rcx
	and rax, rcx
	add rax, rdi
	ret
