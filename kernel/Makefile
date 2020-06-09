
kernel:
	gcc -ffreestanding -c kernel.c -o kernel.o # -m32
	ld -o kernel.img -Ttext 0x7e00 --oformat binary kernel.o # -m elf_i386
	# Sector 1 = bootloader, Sector 2 - (512B TO 64K) = kernel
	truncate --size=65024 kernel.img

clean:
	rm *.o *.img

