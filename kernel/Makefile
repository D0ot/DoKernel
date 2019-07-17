include ../config/config.mk

objects = main.o terminal.c

kernel.bin : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

kernel.debug : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@

kernel.elf : ${objects}
	${CC} ${LINK_FLAGS} -T link.ld -o $@ ${objects}

${objects} : %.o : %.c
	${CC} -c $< -o $@ ${CC_FLAGS}

.PHONY : all obj clean
all : kernel.debug kernel.bin

obj : ${objects}

clean :
	${CLEAN_CMD}