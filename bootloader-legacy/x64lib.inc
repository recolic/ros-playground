
; requires: str.32.inc
; runs in 32bit protection mode

test_cpuid_support:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21) in
    ; the FLAGS register. If we can flip it, CPUID is available.
 
    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax
 
    ; Copy to ECX as well for comparing later on
    mov ecx, eax
 
    ; Flip the ID bit
    xor eax, 1 << 21
 
    ; Copy EAX to FLAGS via the stack
    push eax
    popfd
 
    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax
 
    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the ID bit
    ; back if it was ever flipped).
    push ecx
    popfd
 
    ; Compare EAX and ECX. If they are equal then that means the bit wasn't
    ; flipped, and CPUID isn't supported.
    sub eax, ecx
    ; eax != 0 if CPUID supported, eax==0 if not supported. 
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

test_support_long_mode:
    ; test if possible to enter x64 mode. 
    ; return value in eax. 
    ; return 0 = OK. 1 = CPUID_NOT_AVAIL, 2 = CPUID_EXT_NOT_AVAIL, 3 = LONGMODE_NOT_AVAIL
    call test_cpuid_support
    cmp eax, 0
    jne _cpuid_supported
    mov eax, 1
    ret
_cpuid_supported:
    mov eax, 0x80000000     ; Set the A-register to 0x80000000.
    cpuid                   ; CPU identification.
    cmp eax, 0x80000001     ; Compare the A-register with 0x80000001.
    jb _fail_no_long_mode_2 ; It is less, there is no long mode.
    mov eax, 0x80000001     ; Set the A-register to 0x80000001.
    cpuid                   ; CPU identification.
    test edx, 1 << 29       ; Test if the LM-bit, which is bit 29, is set in the D-register.
    jz _fail_no_long_mode_3 ; They aren't, there is no long mode.
    mov eax, 0
    ret
_fail_no_long_mode_2:
    mov eax, 2
    ret
_fail_no_long_mode_3:
    mov eax, 3
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

enter_long_mode:
    ;;;;;;;;;;;;; section 1: paging
    ; disable paging in 32bit protected mode (if any)
    mov eax, cr0
    and eax, 01111111111111111111111111111111b     ; Clear the PG-bit, which is bit 31. 
    mov cr0, eax

    ; prepare paging for long mode. Using memory 0x1000 - 0x5000, to put 4-level page tables.
    mov edi, 0x1000    ; begin addr
    xor eax, eax
    mov ecx, 0x1000    ; size
    rep stosd          ; memset, 0x1000 * 4Bytes to set this memory area to ZERO.

    ; set PML4T
    mov edi, 0x1000
    mov cr3, edi

    ; fill first entry of PML4T, PDPT, PDT. 
    mov DWORD [edi], 0x2003 ; 0x2003 IS NOT the address. it contains many flags, and another DWORD is zero. Just a trick. 
    mov edi, 0x2000
    mov DWORD [edi], 0x3003
    mov edi, 0x3000
    mov DWORD [edi], 0x4003
    mov edi, 0x4000

    ; fill all entries of the PT. This is a trick, which not setting all bits in entry. 
    ; This page table has 512 4K page, 0x00100000 - 0x00300000.
    mov ebx, 0x00100003 ; BIT M--12 contains PageFrameNumber. 
    mov ecx, 512 ; counter
_set_one_entry:
    mov DWORD [edi], ebx
    add ebx, 0x1000 ; each page is 4K
    add edi, 8 ; each entry is 64bit. 
    loop _set_one_entry

    ; Page table prepared. Enable PAE-paging. 
    mov eax, cr4
    or eax, 1<<5
    mov cr4, eax

    ;;;;;;;;;;;;;;;; Section 2: enter compatibility mode
    mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        ; Read from the model-specific register.
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr                        ; Write to the model-specific register.

    mov eax, cr0                 ; Set the A-register to control register 0.
    or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
    mov cr0, eax                 ; Set control register 0 to the A-register.





