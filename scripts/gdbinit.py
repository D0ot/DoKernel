import gdb
import os
from elftools.elf.elffile import ELFFile


def getSectionAddr(filename, section_name):
    with open(filename, 'rb') as f:
        elffile = ELFFile(f)
        dot_text = elffile.get_section_by_name(section_name)
        if not dot_text:
            print('WARNING: The section:\"{}\" dose not exists in file:\"{}\".'.format(section_name, filename))
            return 0
        
        return dot_text['sh_addr']

def myGDBAutoLoadSymFileWithOffset(filename, offset):
    addr = getSectionAddr(filename, '.text')
    gdb.execute('add-symbol-file ' + filename + ' ' + hex(addr + offset))

def myGDBAutoLoadSymFile(filename):
    myGDBAutoLoadSymFileWithOffset(filename, 0)



def main():
    gdb.execute('target remote localhost:1234')
    gdb.execute('set disassembly-flavor intel')

    myGDBAutoLoadSymFile('debug/boot.debug')
    myGDBAutoLoadSymFile('debug/kernel.debug')
    gdb.execute('dir boot')
    #gdb.execute('b _start')
    gdb.execute('b main')

main()