
# BITS could be 32 or 64.
BITS ?= 64

ifeq ($(BITS), 64)
ARCH = x86_64
else
ARCH = i386
endif

assemble: kernel head
	ld -o kernel.img -fpie -shared --oformat binary image_head.o kernel.o -m elf_$(ARCH)

head:
	nasm -f elf$(BITS) -DTARGET_BITS=$(BITS) image_head.asm -o image_head.o

kernel:
	g++ -ffreestanding -fpie -c kernel.cc -o kernel.o -m$(BITS) -std=c++17

clean:
	rm -f *.o *.img

