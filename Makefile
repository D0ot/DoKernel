include config/config.mk


.PHONY: all clean debug defs

all : defs
	cd boot && make all
	cd kernel && make all

	rm -f build/${BIN}
	cp boot/boot.elf build/img.elf

	
	dd if=/dev/zero of=build/${BIN} count=64
	dd if=boot/boot.bin of=build/${BIN} seek=0 count=2

	# seek=2 because boot.bin is 1K-Byte
	dd conv=notrunc if=kernel/kernel.bin of=build/${BIN} seek=2
	dd if=/dev/null of=build/${BIN} bs=1 count=1 seek=32768


	cp boot/boot.debug debug/
	cp kernel/kernel.debug debug/

clean : 
	cd boot && make clean
	cd kernel && make clean
	rm -f build/*
	rm -f debug/*

defs :
	echo "not impelemented"
	

debug : all
	gnome-terminal --geometry=80x14+0+1200 -- ./scripts/test.sh
	gnome-terminal --geometry=130x80+1600+0 -- ./scripts/debugp.sh 
