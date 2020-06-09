[bits 16]
[org 0x7c00]

mov [_boot_drive_id], dl
jmp _init_prot_mode

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

_init_prot_mode:
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
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


    ; Load the kernel image from boot disk, disk offset 512B to 64KB.
    KERN_ADDR equ 0x7e00
    mov ah, 0x02
    mov ch, 0
    mov dh, 0
    mov cl, 2 ; from the second one,
    mov al, 127 ; read 127 sectors in total.
    mov bx, KERN_ADDR
    int 0x13 ; Set carry on error, and set AL to sectors that actual read.
    jmp _stall

    jc disk_io_error
    mov dl, 0x7f
    cmp dl, al
    jne disk_io_error
    ; kernel successfully loaded now!
    mov ebx, _motd_kern_ok
    call println_vga


_stall:
    jmp $

disk_io_error:
    mov ebx, _motd_disk_error
    call println_vga
    jmp _stall

_motd_disk_error:
    db 'DISK_IO_ERROR', 0x0
_motd_32:
    db '[ENTER X86 MODE SUCC]', 0x0
_motd_kern_ok:
    db '[ENTER X86 MODE SUCC] [LOAD KERN SUCC]', 0x0
_motd_endk:
    db '[ENTER X86 MODE SUCC] [LOAD KERN SUCC] [KERN EXITED]', 0x0
_boot_drive_id:
    db 0x0
    
%include "./mbr_end.inc"    


