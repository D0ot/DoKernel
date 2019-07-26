include ../config/config.mk

objects = boot.o 

boot.bin : boot.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

boot.debug : boot.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@

boot.elf : ${objects} link.ld
	${CC} ${LINK_FLAGS} -T link.ld -o $@ ${objects}


${objects} : %.o : %.asm
	${AS} ${AS_FLAGS} $< -o $@ 

.PHONY : all obj clean 

all : boot.bin boot.debug

obj : ${objects}

clean : 
	${CLEAN_CMD}