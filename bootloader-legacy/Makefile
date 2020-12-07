BITS ?= 64

build:
	nasm -f bin boot.asm -DTARGET_BITS=$(BITS) -o boot.img

run: build
	qemu-system-x86_64 boot.img

clean:
	rm -f boot.img


