meminfo_addr equ 0x8000 ; where to store ARDS first 4 Bytes is number of entries
kernel_addr equ 0x400000 ; where should we load kerenl 
ards_size equ 0x18      ; Address Range Descriptor Structure Size
lib_addr equ 0x9000     ; where .lib_text section are loaded