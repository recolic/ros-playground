#!/usr/bin/fish

set efi_file $argv[1]
set output_file ./output.img
set mountp ./tmp.mount.point

dd if=/dev/zero of=$output_file bs=560M count=1 status=progress
and sgdisk -n 1:0:0 $output_file

and sudo losetup /dev/loop5 $output_file --partscan
and sudo mkfs.vfat -F32 /dev/loop5p1

and rm -rf $mountp ; and mkdir $mountp
and sudo mount /dev/loop5p1 $mountp
and sudo mkdir -p $mountp/efi/boot

and sudo cp $efi_file $mountp/efi/boot/BOOTX64.efi

and sudo umount $mountp
and sudo sync
and sudo losetup -d /dev/loop5

and echo "Success"
or echo "Failed."





