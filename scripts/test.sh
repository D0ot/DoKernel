#!/bin/bash

qemu-system-i386 -hda build/img.bin -gdb tcp::1234 -S -monitor stdio