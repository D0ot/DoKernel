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

    allsize = ABSOLUTE(.);
    .padding : 
    {
        . = 0x8000 + 512 - allsize - 1;
        BYTE(0xcc);
    }

}