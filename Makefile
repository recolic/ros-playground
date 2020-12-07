
default: assemble-legacy

bootloader-legacy:
	$(MAKE) -C bootloader-legacy

bootloader-uefi:
	$(MAKE) -C bootloader-uefi

kernel:
	$(MAKE) -C kernel

.PHONY: bootloader-legacy bootloader-uefi kernel

build: bootloader-legacy bootloader-uefi kernel

assemble-legacy: build
	cat bootloader-legacy/boot.img kernel/kernel.img > legacy.img

assemble-uefi: build

run-legacy: assemble-legacy
	qemu-system-x86_64 legacy.img


