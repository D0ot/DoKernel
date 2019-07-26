SECTIONS
{
    /* jump to here to run */
    . = 0x7C00;
    .text :
    {
        boot.o(.text)
    }
    
    .data :
    {
        boot.o(.data)
        boot.o(.rodata)
    }

    allsize = ABSOLUTE(.);
    .mbrcheck : 
    {
        . = 0x7C00 + 512 - allsize - 2;
        boot.o(.mbrcheck)

    }
    
    . = 0x9000;
    .text.1 : AT(0x7c00 + 0x200)
    {
        boot.o(.text.1)
        . = ALIGN(0x200) - 1;
        BYTE(0xcc)
    }




}
