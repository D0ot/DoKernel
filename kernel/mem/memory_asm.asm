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
; (uint16_t arg_cs, uint16_t arg_ds)
global memory_init_flush_0
memory_init_flush_0:
    
    jmp dword 0x0008:flush

flush:
    ret
    
    