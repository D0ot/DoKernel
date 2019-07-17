# This is how to get .text section address
# We can use it in gdbinit.py

import sh
import os
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section

def main(filename):
    with open(filename, 'rb') as f:
        elffile = ELFFile(f)
        text_section = elffile.get_section_by_name('.text')
        if not text_section:
            print('Not .text section found')
            exit(1)
        addr = text_section['sh_addr']
        print('Address :', addr)




if __name__ == '__main__':
    main('boot.elf')    