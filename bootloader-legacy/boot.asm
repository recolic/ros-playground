; This code file would be compiled and placed in MBR first block. 

[bits 16]
[org 0x7c00]

KERN_ADDR equ 0x7e00

mov [_boot_drive_id], dl
jmp _load_kern
; jmp _init_prot_mode

%include "./str.16.inc"



gdt_begin:

gdt_entry_0:
    dd 0x0, 0x0

gdt_entry_1:
    ; code
    dw 0xffff ; limit[16:0]
    dw 0x0 ; base[16:0]
    db 0x0 ; base[24:16]
    db 10011010b ; flags
    db 11001111b ; flags : limit[]
    db 0x0 ; base[32:24]
    
gdt_entry_2:
    ; data
    dw 0xffff ; limit[16:0]
    dw 0x0 ; base[16:0]
    db 0x0 ; base[24:16]
    db 10010010b ; flags
    db 11001111b ; flags : limit[]
    db 0x0 ; base[32:24]

gdt_end: 

gdt_desc:
    dw gdt_end - gdt_begin - 1
    dd gdt_begin

; Segment register values.
CODE_SEG_OFFSET equ gdt_entry_1 - gdt_begin
DATA_SEG_OFFSET equ gdt_entry_2 - gdt_begin

_load_kern:
    ; Load the kernel image from boot disk, disk offset 512B to 64KB.
    ; This is a 16bit real mode function.
    mov ah, 0x02
    mov dl, [_boot_drive_id]
    mov ch, 0
    mov dh, 0
    mov cl, 2 ; from the second one,
    mov al, 127 ; read 127 sectors in total.
    mov bx, KERN_ADDR
    int 0x13 ; Set carry on error, and set AL to sectors that actual read.

    jc disk_io_error
    mov dl, 0x7f
    cmp dl, al
    jne disk_io_error
    ; kernel successfully loaded now!
    mov bx, _motd_kern_ok
    call println_bios
    jmp _init_prot_mode

disk_io_error:
    mov bx, _motd_disk_error
    call println_bios
    jmp $




_init_prot_mode:
    cli ; turn off interrupt, until we set interrupt vector for 32b protected mode.
    lgdt [gdt_desc] ; set GDT,
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax ; and set CR0.
    ; We're almost in 32bit protected mode now. Flush the pipeline by far jump. 
    jmp CODE_SEG_OFFSET:_prot_begin

[bits 32]
%include "./str.32.inc"
_prot_begin:
    mov ax, DATA_SEG_OFFSET
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; 600KB free space here, until 0x7c00 + 0x200 byte (MBR sector 0)
    mov esp, ebp
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 32BIT PROTECTED MODE BEGIN ;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov ebx, _motd_32
    call println_vga

%if TARGET_BITS == 64
    ; Test if 64bit available
    call test_support_long_mode
    cmp eax, 0
    je _test_passed
    mov ebx, _motd_no_long_mode
    call println_vga
    jmp _call_kern_32
_test_passed:

    jmp inline_enter_long_mode
%include "./inline_x64lib.inc"
    jmp _call_kern_64
%else
    jmp _call_kern_32
%endif

[bits 32]
_call_kern_32:
    ; Enter the kernel. This should never return.
    call KERN_ADDR

    ; Kernel returns.
    mov ebx, _motd_endk
    call println_vga
    jmp $

[bits 64]
_call_kern_64:
    ; Enter the kernel. This should never return.
    call KERN_ADDR

    ; Kernel returns.
    ;mov ebx, _motd_endk
    ;call println_vga
    ; TODO: add 64bit println_vga and error msg
    jmp $


; To save binary space
_motd_disk_error:
    db 'MED', 0x0
_motd_32:
    db 'M32', 0x0
_motd_kern_ok:
    db 'MKN', 0x0
_motd_endk:
    db 'MEK', 0x0
_motd_no_long_mode:
    db 'MNL', 0x0
_boot_drive_id:
    db 0x0

_motd_debug_point:
    db 'MDB', 0x0
    
%include "./mbr_end.inc"    


