OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(pre_main)

SECTIONS
{
    . = 0x8000;

    .entry :
    {
        jumper.o(.text) /* it will  call main() */
    }

    . = ALIGN(4);
    .text :
    {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    . = ALIGN(4);
    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }


    . = ALIGN(4);
    .bss :
    {
        *(.bss)
    }


    . = ALIGN(4);
    .data :
    {
        *(.data)
    }


    .check :
    {
        . = .;
        BYTE(0xcc);
    }

    /DISCARD/ : 
    {
        *(.eh_frame)
    }

}