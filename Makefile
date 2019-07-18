include config/config.mk


.PHONY: all clean debug

all : 
	cd boot && make all
	cd kernel && make all

	rm -f build/${BIN}
	cp boot/boot.elf build/img.elf

	dd if=/dev/zero of=build/${BIN} count=64
	dd if=boot/boot.bin of=build/${BIN} seek=0 count=1
	dd conv=notrunc if=kernel/kernel.bin of=build/${BIN} seek=1


	cp boot/boot.debug debug/
	cp kernel/kernel.debug debug/

clean : 
	cd boot && make clean
	cd kernel && make clean
	rm -f build/*
	rm -f debug/*
	

debug : all
	gnome-terminal --geometry=80x14+0+1200 -- ./scripts/test.sh
	gnome-terminal --geometry=130x80+1600+0 -- ./scripts/debugp.sh 
