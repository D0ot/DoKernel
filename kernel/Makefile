include ../config/config.mk

objects = main.o

kernel.bin : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

kernel.debug : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@

kernel.elf : ${objects}
	${CC} ${LINK_FLAGS} -T link.ld -o $@ ${objects}

main.o : main.c
	${CC} -c main.c -o main.o -std=gnu99 -ffreestanding -O0 -Wall -Wextra -g

.PHONY : all obj clean
all : kernel.debug kernel.bin

obj : ${objects}

clean :
	${CLEAN_CMD}