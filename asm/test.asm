;
; A simple boot sector program that loops forever.
;
[org 0x7c00]

mov ebx, str
call println_vga
jmp $

println_bios:
    ; Arg0: addr in register bx
    ; while true:
    ;     if [bx] != NULL:
    ;         print [bx]; ++bx
    mov ah, 0x0e
_loop_begin:
    mov al, [bx]
    cmp al, 0x0
    je _loop_exit
    int 0x10
    inc bx
    jmp _loop_begin
_loop_exit:
    ret
    
; [bits 32]
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
    

str:
    db 'This is Recolic!', 0x0

times 510-($-$$) db 0
dw 0xaa55


