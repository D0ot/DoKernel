#!/bin/bash

qemu-system-i386 -m 128M -hda build/img.bin -gdb tcp::1234 -S -monitor stdio -serial file:com1.log

# used for debug serial port 
# qemu-system-i386 -hda build/img.bin -gdb tcp::1234 -S -serial stdio 