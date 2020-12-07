
# BITS could be 32 or 64.
BITS ?= 64

ifeq ($(BITS), 64)
ARCH = x86_64
else
ARCH = i386
endif

assemble: kernel head
	ld -o kernel.img -Ttext 0x7e00 --oformat binary image_head.o kernel.o -m elf_$(ARCH)
	# Sector 1 = bootloader, Sector 2 - (512B TO 64K) = kernel
	# Extend kernel.img to correct size. 
	test $$(stat -c %s kernel.img) -le 65024
	truncate --size=65024 kernel.img

head:
	nasm -f elf$(BITS) -DTARGET_BITS=$(BITS) image_head.asm -o image_head.o

kernel:
	g++ -ffreestanding -fno-pie -c kernel.cc -o kernel.o -m$(BITS) -std=c++17

clean:
	rm -f *.o *.img

