
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
	# Sector 1 = bootloader, Sector 2 - (512B TO 64K) = kernel
	# Extend kernel.img to correct size. 
	test $$(stat -c %s kernel/kernel.img) -le 65024
	truncate --size=65024 kernel/kernel.img
	cat bootloader-legacy/boot.img kernel/kernel.img > legacy.img

assemble-uefi: build

run-legacy: assemble-legacy
	qemu-system-x86_64 legacy.img

clean:
	$(MAKE) -C bootloader-legacy clean
	# $(MAKE) -C bootloader-uefi clean
	$(MAKE) -C kernel clean


