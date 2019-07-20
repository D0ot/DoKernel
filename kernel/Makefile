include ../config/config.mk

objects = main.o terminal.o jumper.o disk.o common.o uart.o kdebug.o

kernel.bin : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

kernel.debug : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@

#kernel.elf : ${objects} link.ld
#	${CC} ${LINK_FLAGS} -T link.ld -o $@ ${objects}

kernel.elf : ${objects} link.ld
	${LD} -T link.ld -N -e pre_main -o $@ ${objects}

${objects} : %.o : %.c
	${CC} -c $< -o $@ ${CC_FLAGS}

.PHONY : all obj clean
all : kernel.debug kernel.bin

obj : ${objects}

clean :
	${CLEAN_CMD}