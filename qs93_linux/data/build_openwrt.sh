#!/bin/sh

cd imx_openwrt
cp ../variantfiles/configs/.config .
make -j8
