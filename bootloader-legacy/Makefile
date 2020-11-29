build:
	nasm -f bin boot.asm -o boot.img

build_x86:

build_x86_64:


run: build
	qemu-system-x86_64 boot.img

