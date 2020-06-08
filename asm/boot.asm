[bits 16]
[org 0x7c00]

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

_dead_loop:
    mov ebx, _motd_32
    call println_vga
    mov ebx, _motd_32p
    call println_vga
    jmp _dead_loop

_motd_32:
    db '----- CPU is in INTEL x86 protected mode now -----', 0x0
_motd_32p:
    db '+++++ CPU is in INTEL x86 protected mode now +++++', 0x0
    
%include "./mbr_end.inc"    


