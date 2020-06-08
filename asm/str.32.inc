
VGA_BEGIN_ADDR equ 0xb8000
println_vga:
    ; Arg0: addr in register ebx
    pusha
    mov edx, VGA_BEGIN_ADDR
    mov ah, 0x0f
_loop_begin_1:
    mov al, [ebx]
    cmp al, 0x0
    je _loop_exit_1
    mov [edx], ax
    inc ebx
    add edx, 2
    jmp _loop_begin_1
_loop_exit_1:
    popa
    ret


