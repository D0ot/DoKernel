data_addr equ 0x7e00


SECTION .mbrcheck
            db 0x55
            db 0xaa

SECTION .rodata
gdt_size    dw 0
gdt_base    dd 0x00007e00

            [BITS 16]
        
SECTION .text
global _start
_start:
            xor ax, ax
            mov ss, ax
            mov sp, 0x7c00
            mov bx, 0x7e00
            
            mov dword [bx + 0x00], 0
            mov dword [bx + 0x04], 0
            
            mov dword [bx + 0x08], 0x0000ffff
            mov dword [bx + 0x0c], 0x00cf9a00

            mov dword [bx + 0x10], 0x0000ffff
            mov dword [bx + 0x14], 0x00cf9200

            mov word [cs: gdt_size], 24-1

            lgdt [cs: gdt_size]

            in al, 0x92
            or al, 0000_0010b
            out 0x92, al

            cli

            mov eax, cr0
            or eax, 1
            mov cr0, eax

            jmp 0x0008:flush

            [BITS 32]
    flush:
            mov ax, 0x10
            mov ds, ax
            mov ss, ax
            mov es, ax
            mov esp, 0x7C00

            mov ax, 0x01
            mov bx, 0x8000
            mov cx, 0xff
            call _disk_read

            mov ax, 0x02
            mov bx, 0x8200
            mov cx, 0xff
            call _disk_read

            ;mov ax, 0x03
            ;mov bx, 0x8400
            ;mov cx, 0xff
            ;call _disk_read

            mov esp, 0x17c00
            mov ebp, 0x17d00

            jmp 0x0008:0x8000
    _inf_loop_32bit:
            inc eax
            hlt
            jmp _inf_loop_32bit


;   ax , index of sector to read, only 7bit works
;   bx , buffer to store data
;   cx , size of word(2-byte) to read


_disk_read: 
            push ax
            mov al, 1
            mov dx, 0x1F2
            out dx, al
            pop ax

            mov dx, 0x1F3
            out dx, al

            inc dx      
            mov al, ah
            out dx, al

            inc dx     
            mov ax, 0x00
            out dx, al

            inc dx      
            mov al, 0xE0
            out dx, al

            inc dx      
            mov al, 0x20
            out dx, al
    dr_wait:
            in al, dx
            and al, 0x88
            cmp al, 0x08
            jnz dr_wait

            push cx
            mov dx, 0x1F0

    dr_read:
            in ax, dx
            mov [ds:bx], ax
            add bx, 2
            loop dr_read

            pop ax
            mov cx, 512
            sub cx, ax

    dr_read_zero: 
            in ax, dx
            loop dr_read_zero

            ret
