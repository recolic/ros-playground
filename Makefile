
build:
	cc -c boot.c -o boot.o -m32
	ld -o boot.img -Ttext 0x0 --oformat binary boot.o -m elf_i386
	# objcopy -O binary boot.o boot.img
	# [[ $$(stat boot.img --printf='%s') -lt 510 ]]
	truncate --size=510 boot.img
	printf '\x55\xAA' >> boot.img

run: build
	qemu-system-x86_64 boot.img


clean:
	rm *.o

