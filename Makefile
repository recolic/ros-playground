
BITS ?= 64

default: assemble-legacy

bootloader-legacy:
	BITS=$(BITS) $(MAKE) -C bootloader-legacy

bootloader-uefi:
	# BITS=$(BITS) $(MAKE) -C bootloader-uefi

kernel:
	BITS=$(BITS) $(MAKE) -C kernel

.PHONY: bootloader-legacy bootloader-uefi kernel

build: bootloader-legacy bootloader-uefi kernel

assemble-legacy: build
	cat bootloader-legacy/boot.img kernel/kernel.img > legacy.img

assemble-uefi: build

run-legacy: assemble-legacy
	qemu-system-x86_64 legacy.img

clean:
	$(MAKE) -C bootloader-legacy clean
	# $(MAKE) -C bootloader-uefi clean
	$(MAKE) -C kernel clean


