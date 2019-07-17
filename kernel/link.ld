SECTIONS
{
    . = 0x8000;

    .text ALIGN(0x200):
    {
        main.o(.text)
        *(.text)
    }

    .data ALIGN(0x200):
    {
        *(.data)
    }

    .rodata ALIGN(0x200):
    {
        *(.data)
    }

    .bss ALIGN(0x200):
    {
        *(.bss)
    }

    .check ALIGN(0x200):
    {
        . = .;
        BYTE(0xcc);
    }

}