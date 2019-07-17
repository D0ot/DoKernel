SECTIONS
{
    . = 0x8000;

    .text :
    {
        main.o(.text)
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .rodata :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    .check :
    {
        . = .;
        BYTE(0xcc);
    }

}