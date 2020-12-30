CFLAGS=-target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -I./efi
LDFLAGS=-target x86_64-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link

build:
	clang $(CFLAGS) -o main.o -c main.c
	clang $(LDFLAGS) -o main.efi main.o


