
default: assemble

bootloader:
	$(MAKE) -C bootloader

kernel:
	$(MAKE) -C kernel

.PHONY: bootloader kernel

build: bootloader kernel

assemble: build
	cat bootloader/boot.img kernel/kernel.img > disk.img

run: assemble
	qemu-system-x86_64 disk.img


