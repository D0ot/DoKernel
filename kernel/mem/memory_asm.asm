SECTION .text
global memory_switch_gdt

; can use x86_lgdt to replace it.
memory_switch_gdt:
    mov eax, [esp + 0x04]
    lgdt [ds:eax]
    ret 

; to use kernel segment
; flush the segment selector cache
; 2 arguments, segment for kerenl
; (uint16_t cs, uint16_t ds)
memory_init_flush_0:
    
    mov ax, [esp + 0x08]
    mov bx, [esp + 0x04]

    mov ds, bx
    mov es, bx
    mov ss, bx

.memory_init_flush_0_lab:
    

    ret
    
    