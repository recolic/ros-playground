
assemble: kernel head
	ld -o kernel.img -Ttext 0x7e00 --oformat binary image_head.o kernel.o -m elf_i386
	# Sector 1 = bootloader, Sector 2 - (512B TO 64K) = kernel
	truncate --size=65024 kernel.img

head:
	nasm -f elf image_head.asm -o image_head.o

kernel:
	gcc -ffreestanding -fno-pie -c kernel.c -o kernel.o -m32

clean:
	rm *.o *.img

