SECTION .text
global memory_switch_gdt

; can use x86_lgdt to replace it.
memory_switch_gdt:
    mov eax, [esp + 0x04]
    lgdt [ds:eax]
    ret 

; to use kernel segment
; flush the segment selector cache
memory_init_flush_0:
    

    ret
    
    