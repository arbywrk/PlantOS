#!/bin/bash

nasm -f elf32 src/kernel.asm -o kasm.o
gcc -m32 -c src/kmain.c -o kc.o
ld -m elf_i386 -T src/link.ld -o kernel-1 kasm.o kc.o
qemu-img create disk.img 128M\

sudo losetup -Pf disk.img\

lsblk
sudo fdisk /dev/loop0
sudo mkfs.ext2 /dev/loop0p1\

mkdir mnt
sudo mount /dev/loop0p1 mnt\
sudo mkdir -p mnt/boot/grub\

sudo cp kernel-1 mnt/boot/\

sudo nvim mnt/boot/grub/grub.cfg

sudo grub-install \\
  --target=i386-pc \\
  --boot-directory=mnt/boot \\
  /dev/loop0\

sudo pacman -S grub
sudo grub-install \\
  --target=i386-pc \\
  --boot-directory=mnt/boot \\
  /dev/loop0\

sudo umount mnt\
sudo losetup -d /dev/loop0\

qemu-system-i386 \\
  -drive file=disk.img,format=raw \\
  -m 256M\

