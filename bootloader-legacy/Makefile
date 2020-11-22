build:
	nasm -f bin boot.asm -o boot.img

run: build
	qemu-system-x86_64 boot.img

