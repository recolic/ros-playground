CFLAGS="-target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -I/usr/include/efi -I/usr/include/efi/x86_64"

LDFLAGS="-target x86_64-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link"

clang $CFLAGS -o main.o -c ehelo.demo.c
clang $LDFLAGS -o main.efi main.o


