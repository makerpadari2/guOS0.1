global start
extern long_mode_start

section .text
bits 32

start:
    mov esp, stack_top ; Address of the top of the stack is moved into the esp register

    call check_multiboot ; Confirm loading by multiboot bootloader
    call check_cpuid ; Check for CPU ID
    call check_long_mode ; Use CPU ID to check for long mode support

    ; Map virtual addresses to physical addresses to avoid working directly
    ; with physical addresses in RAM.
    ; To enter 64 bit long mode virtual memory has to be implemented with Paging
    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer] ; Load global descriptor table
    jmp gdt64.code_segment:long_mode_start ; load code segment

    
    hlt

check_multiboot:
    cmp eax, 0x36d76289 ; Check eax register for multiboot confirm value
    jne .no_multiboot ; Jump to no_multiboot label if comparison fits
    ret
.no_multiboot:
    mov al, "M" ; Use the error code if not multiboot compatible
    jmp error

check_cpuid:
    ; Attempts to flip the CPUID bit of the flags register
    pushfd ; Push flags register onto the stack
    pop eax ; Pop off the stack into the eax register
    mov ecx, eax ; Copy into exc register
    xor eax, 1 << 21 ; Flip the bit in the eax register
    push eax
    popfd 
    pushfd
    pop eax ; Pop from eax register, if ID was remains flipped then CPU supports CPUID
    push ecx
    popfd ; Resets flags back to what they were before the instructions
    cmp eax, ecx 
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "C" ; Error for no CPUID
    jmp error

check_long_mode:
    mov eax, 0x80000000
    ; Takes eax register value as argument, when it reads the number it will store a value back into eax
    ; which will be greater than the value given if the CPU supports Extended Processor Info
    cpuid   
    cmp eax, 0x80000001 ; If eax is less than the value then Extended Processor Info is not supported
    jb .no_long_mode

    mov eax, 0x80000001 ; Check if long mode is available
    cpuid
    test edx, 1 << 29 ; If lm bit is set in the edx register it means long mode is supported
    jz .no_long_mode
    
    ret
.no_long_mode:
    mov al, "L"
    jmp error

setup_page_tables:
    mov eax, page_table_l3
    or eax, 0b11 ; present, writable flags
    mov [page_table_l4], eax ; Pages with added flags added to the l4 table

    mov eax, page_table_l2
    or eax, 0b11 ; present, writable
    mov [page_table_l3], eax

    mov ecx, 0 ; counter
.loop:

    mov eax, 0x200000 ; Map a 2Mib page
    mul ecx
    or eax, 0b10000011 ; present, writable, huge page
    mov [page_table_l2 + ecx * 8], eax ; Entry is put into the l2 table

    inc ecx ; increment counter
    cmp ecx, 512 ; checks if the whole table is mapped
    jne .loop ; if not, continue

    ret

enable_paging:
    ; pass page table location to cpu
    mov eax, page_table_l4 ; value of l4 table is moved into the eax register
    mov cr3, eax ; Value of eax is copied into the cr3 register

    ; enable  PAE (Physical Address Extention), necessary for 64 bit paging
    mov eax, cr4
    or eax, 1 << 5 ; Enable 5th bit which is the bit for the PAE flag
    mov cr4, eax ; Save changes into the cr4 register

    ; enable long mode
    mov ecx, 0xC0000080
    rdmsr   ; Read model specific register instruction, will load value of the efr register into the eax register
    or eax, 1 << 8 ; Enable the long mode flag at bit 8
    wrmsr   ; Write it back into the model specific register

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31 ; Enables the paging flag in the cr0 register
    mov cr0, eax

    ret


error:
    ;print "ERR: X" where X is the error code
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte [0xb800a], al
    hlt


section .bss ; bss section contains statically allocated variables
align 8192 ; Aligns all tables to 4 kb
    ; Page tables define all the mappings the CPU looks at whenever reading
    ; or writing to memory
    page_table_l4:
        resb 8192
    page_table_l3:
        resb 8192
    page_table_l2:
        resb 8192
    stack_bottom: 
        resb 8192 * 4 ; 16 kb of memory reserved for the stack
    stack_top:

section .rodata
gdt64: ; 64 bit global discriptor table
    dq 0 ; zero entry
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64