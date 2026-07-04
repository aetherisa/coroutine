format ELF64

section '.text'

;; rax: coroutine's result
public forward_rax 
forward_rax:
	mov rdi, rax
	ret
