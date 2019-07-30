; constant definition
meminfo_addr equ 0x8000 ; where to store ARDS first 4 Bytes is number of entries
kernel_addr equ 0x400000 ; where should we load kerenl 
ards_size equ 0x18      ; Address Range Descriptor Structure Size
lib_addr equ 0x9000     ; where .lib_text section are loaded
page_dir_addr equ 0x100000  ; where Page Directory entries are stored

; entries in page directory
PDE_0 equ (0x0 << 22) + (10011011b)
PDE_1 equ (0x1 << 22) + (10011011b)
PDE_511 equ (0x200 << 22) + (10011011b)