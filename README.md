# notes

## register usage rule

volatile (caller saved): rax, rcx, rdx

non-volatile: others

arguments and return value: rax, rcx, rdx, stack

## bootloader(legacy) memory model

- common

0x7c00 - 0x7e00 : bootloader.img, executes from beginning 0x7c00  
0x7e00 - 0x17c00 : kernel.img, called by bootloader, from beginning 0x7e00

- bios age

![](../res/1.png)

- 32bit protected mode age

Just used this mode to launch 64bit long mode. No paging enabled. 

- 64bit long mode age

0x1000 - 0x5000 : 64bit long mode paging table**s**
0x00000000 - 0x00200000 : 512 * 4K pages, before the kernel initializing page table in C++.

(0x0 - 0x100000 are used by kernel structures, 0x100000-0x200000 are used by kalloc(), defined in kutils.hpp)

## bootloader(legacy) disk model

0-511Byte : bootloader.img from boot.asm, MBR flag  
512B - 64KiB : kernel.img. executes from 512Byte first instruction. 

## works and TODO

Currently, paging for 32bit bootloader not implemented. Not planned. 

kernel currently working on both 32bit(without paging) and 64bit mode. But in the future, will work only in 64bit mode. 

