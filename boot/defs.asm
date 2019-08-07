; constant definition
meminfo_addr equ 0x8000 ; where to store ARDS first 4 Bytes is number of entries
                        ; physical address

kernel_addr equ 0x400000    ; where should we load kerenl
                            ; physical address

ards_size equ 0x18      ; Address Range Descriptor Structure Size
lib_addr equ 0x9000     ; where .lib_text section are loaded, only for boot.asm, pyhsical address

page_dir_addr equ 0x100000  ; where Page Directory entries are stored
                            ; surely physical address

high_base equ 0x80000000    ; high base of kernel, linear address


; entries in page directory
PDE_0 equ (0x0 << 22) + (10011011b)
PDE_1 equ (0x1 << 22) + (10011011b)

; first 4M at 2G 
; here 0x80000000 + 0x80000 is meminfo_addr
PDE_512 equ (0x0 << 22) + (10011011b)

; second 4M at 2G  
; here is kernel's base address (linear address)
; real physical address is at 0x400000
PDE_513 equ (0x1 << 22) + (10011011b)