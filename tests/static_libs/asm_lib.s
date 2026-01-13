section .data
    msg db "Hello, world!", 0xa
    len equ $ - msg

section .text
    global print

print:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall
    ret
