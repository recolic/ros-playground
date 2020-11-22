
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

