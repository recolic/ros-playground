
kernel:
	gcc -ffreestanding -c kernel.c -o kernel.o # -m32
	ld -o kernel.img -Ttext 0x1000 --oformat binary kernel.o # -m elf_i386

clean:
	rm *.o *.img

