section .text
    extern print
    global _start

_start:
    call print

    mov rax, 60                ; sys_exit system call number
    xor rdi, rdi               ; Exit code 0
    syscall
