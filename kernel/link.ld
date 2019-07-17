SECTIONS
{
    . = 0x8000;

    .text :
    {
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    allsize = ABSOLUTE(.);
    .padding : 
    {
        . = 0x8000 + 512 - allsize - 1;
        BYTE(0xcc);
    }

}