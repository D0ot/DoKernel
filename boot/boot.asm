%include "defs.asm"

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
            mov es, ax 
            mov sp, 0x7c00

            mov ax, 1
            mov bx, lib_addr
            mov cx, 1
            call _disk_read_16

            mov di, meminfo_addr
            clc
            call _get_meminfo
            
            mov ax, 1


            xor ax, ax
            mov ss, ax
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


try_read_disk:
            mov ax, 0x20
            mov ebx, kernel_addr 
            mov ecx, 0x2
            mov bp, 0
            mov si, 0
            mov di, 0
            call _disk_read
            jc try_read_disk

            mov eax, cr4
            or eax, (1 << 4) 
            mov cr4, eax    ;enable CR4.PSE to enable 4M-Byte Page size

            mov eax, 0x01
            cpuid
            test edx, (1 << 16) ; if not support PAT(page-attribute table), jumping to inf_loop
            jz _inf_loop_32bit

            ; set PDE table address
            mov eax, ((page_dir_addr) + (11b << 3))
            mov cr3, eax

            ; config PDE
            mov dword [page_dir_addr], PDE_0
            mov dword [page_dir_addr + 1 * 4], PDE_1
            mov dword [page_dir_addr + 512 * 4], PDE_512
            mov dword [page_dir_addr + 513 * 4], PDE_513

            ; enable Paging

            mov eax, cr0
            or eax, (1b << 31)
            mov cr0, eax

            mov esp, 0x7c00
            mov ebp, 0x7c00

            jmp 0x0008:(high_base + kernel_addr)
    _inf_loop_32bit:
            inc eax
            hlt
            jmp _inf_loop_32bit

;   ax , number of logical sectors to read. 8 bits are valid, 0 indicates 256 sectors
;   bx , buffer to store data, it is an address, 16bit are all valid 
;   cx , index of sector to read, lower 16 bits are valid

[BITS 16]
_disk_read_16: 
            mov dx, 0x1F2
            out dx, al
            push ax

            mov al, cl
            mov dx, 0x1F3
            out dx, al          ; LBA 7 - 0

            mov al, ch 
            inc dx 
            out dx, al          ; LBA 15 - 8

            inc dx     
            mov al, 0
            out dx, al          ; LBA 23 - 16

            inc dx
            mov al, 0xE0        
            out dx, al          ; LBA mode and LBA 27 - 24  

            pop ax
            mov cx, ax          ; now cx saves the number of sectors to read

            inc dx      
            mov al, 0x20
            out dx, al

    dr_read_0_16:
            push cx
            mov dx, 0x1f7
    dr_wait_16:

            in al, dx
            test al, 0x80
            jnz dr_wait_16

            test al, 0x01
            jnz dr_read_error_ret_16
            
            test al, 0x08
            jz dr_wait_16
            
            mov dx, 0x1F0
            mov cx, 256 
    dr_read_loop_16:
            in ax, dx
            mov [bx], ax
            add bx, 2
            loop dr_read_loop_16

            pop cx
            loop dr_read_0_16

            clc
            ret
    dr_read_error_ret_16:
            stc
            ret

SECTION .text.1

;   ax , number of logical sectors to read. 8 bits are valid, 0 indicates 256 sectors
;   ebx , buffer to store data, it is an address, 32bit are all valid 
;   ecx , index of sector to read, lower 28 bits are valid
[Bits 32]
_disk_read: 
            mov dx, 0x1F2
            out dx, al
            push ax

            mov al, cl
            mov dx, 0x1F3
            out dx, al          ; LBA 7 - 0

            mov al, ch 
            inc dx      
            mov al, ah
            out dx, al          ; LBA 15 - 8

            shr ecx, 0x10       ; shift the ecx 16bits

            inc dx     
            mov al, cl
            out dx, al          ; LBA 23 - 16

            inc dx
            and ch, 0x0F
            mov al, 0xE0        
            or al, ch 
            out dx, al          ; LBA mode and LBA 27 - 24  

            pop ax
            mov cx, ax          ; now cx saves the number of sectors to read

            inc dx      
            mov al, 0x20
            out dx, al

    dr_read_0:
            push cx
            mov dx, 0x1f7
    dr_wait:

            in al, dx
            test al, 0x80
            jnz dr_wait

            test al, 0x01
            jnz dr_read_error_ret
            
            test al, 0x08
            jz dr_wait
            
            mov dx, 0x1F0
            mov cx, 256 
    dr_read_loop:
            in ax, dx
            mov [ebx], ax
            add ebx, 2
            loop dr_read_loop

            pop cx
            loop dr_read_0

            clc
            ret
    dr_read_error_ret:
            stc
            ret

; \brief get memory infomation of the machine
; \param es:di is buffer, first 4 bytes is numbre of entries in list, the following it is ARDS entries
[BITS 16]
global _get_meminfo
_get_meminfo:
            xor bp, bp

            mov eax, 0xe820
            xor ebx, ebx
            mov ecx, ards_size 
            mov edx, 0x534d4150
            mov es, bx
            push di
            add di, 4
            mov [es:di + 20], dword 1       ; for ACPI 3.0 Extended Capibility, force it to be valid

            clc                             ; for later error detect, clear carry flag
            int 0x15
            jc meminfo_failed               ; being stuck here indicates failure on reading memory info    

            mov edx, 0x534d4150
            cmp edx, eax
            jne meminfo_failed              ; INT 0x15 returned eax is not 0x534d4150 'SMAP'
            
            cmp ebx, 0
            je meminfo_failed               ; get ebx as 0, implies list is only 1 entry long, we see it as an error
            jmp meminfo_entrycheck

    meminfo_loop:
            mov eax, 0xe820
            mov [es:di + 20], dword 1
            int 0x15
            jc meminfo_success
            mov edx, 0x534d4150
            
    meminfo_entrycheck:
            jcxz meminfo_skip               ; ecx is zero, skip this entry
            cmp cl, 20
            jbe meminfo_notext
            test byte [es:di + 20], 1
            je meminfo_skip                 ; ACPI 3.0 Extended 

    meminfo_notext:
            mov ecx, [es:di + 8]            ; length_low bitwise_or length_high, if it is zero, it is not a vaild entry, skip it.
            or ecx, [es:di + 12]
            jz meminfo_skip
            inc bp
            add di, 24

    meminfo_skip:
            test ebx, ebx                   ; if ebx is 0, we reached the last entry
            jne meminfo_loop

    meminfo_success:
            pop di
            mov [di], bp
            ret
    meminfo_failed:
            jmp meminfo_failed
 
; if ecx is zero, skip it
; if ebx is zero, end it
; if, after int 15h , carry flag is set, end it

