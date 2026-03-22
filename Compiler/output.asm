section .data
x: dq 0

section .text
global _start

_start:
    mov rax, 10
    mov [x], rax
    mov rax, [x]
    ; print rax (not implemented fully)

    ret
