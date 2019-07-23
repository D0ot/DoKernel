DIR_PATH=/home/doot/sysmaking/dokernel

TOOLS_PREFIX = /usr/local/cross/bin/i686-elf-

CC = ${TOOLS_PREFIX}gcc
CXX = ${TOOLS_PREFIX}g++
LD = ${TOOLS_PREIFX}ld
AS = nasm 
OBJCOPY = ${TOOLS_PREIFX}objcopy

BIN = img.bin


AS_FLAGS = -f elf32 
LINK_FLAGS = -ffreestanding -nostdlib -g
CC_FLAGS = -std=gnu11 -ffreestanding -m32 -masm=intel -Wall -Wextra -g -O2

OBJCOPY_GENBIN_FLAGS = -O binary -S
OBJCOPY_GENSYM_FLAGS = --only-keep-debug


CLEAN_CMD = rm -f *.elf *.bin *.o *.debug

