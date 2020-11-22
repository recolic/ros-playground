#!/usr/bin/fish

set efi_file $argv[1]
set kernel_file $argv[2]
if test (count $argv) -lt 2
    echo "Usage: $argv[0] <efi_file> <kernel_file>"
    exit 1
end

set output_file ./disk.img
set mountp ./tmp.mount.point
set disk_size 256M

# Create disk image
dd if=/dev/zero of=$output_file bs=$disk_size count=1 status=progress
and sgdisk -n 1:0:+128M $output_file # EFI system partition

# Format partitions
and sudo losetup /dev/loop5 $output_file --partscan
and sudo mkfs.fat -s2 -F32 /dev/loop5p1

# Mount EFI partition
and rm -rf $mountp ; and mkdir $mountp
and sudo mount /dev/loop5p1 $mountp
and sudo mkdir -p $mountp/efi/boot

# load files
and sudo cp $efi_file $mountp/efi/boot/BOOTX64.efi
and sudo cp $kernel_file $mountp/kernel.efi

# Cleanup
and sudo umount $mountp
and sudo sync
and sudo losetup -d /dev/loop5
and rmdir $mountp

and echo "Success"
or echo "Failed."





