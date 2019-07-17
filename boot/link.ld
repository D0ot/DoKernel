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


}
