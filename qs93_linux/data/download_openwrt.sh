#!/bin/sh

#git clone https://github.com/nxp-imx/imx_openwrt.git
git clone https://github.com/ras14k/imx_openwrt.git

cd imx_openwrt
#git checkout imx-openwrt-23.05
git checkout qs93-5210

./scripts/feeds update -a
./scripts/feeds install -a

