#!/bin/bash

# update path to kernel to match target image
qemu-system-aarch64 -m 1024 -smp 2 -cpu cortex-a57 -M virt -nographic -kernel imx_openwrt/bin/targets/imx/cortexa55/openwrt-imx-cortexa55-imx93-qs93-5210-initramfs-kernel.bin

# add these params to get access to luci website 
#  -net nic -net user,hostfwd=tcp::8880-:80

