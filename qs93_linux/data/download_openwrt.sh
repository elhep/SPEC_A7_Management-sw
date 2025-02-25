#!/bin/sh

git clone https://github.com/nxp-imx/imx_openwrt.git

cd imx_openwrt
git checkout imx-openwrt-23.05

./scripts/feeds update -a
./scripts/feeds install -a

