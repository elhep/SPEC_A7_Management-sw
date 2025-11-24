#!/bin/sh

#config_file=".config_minimal"
#config_file=".config_full"
config_file=".config_openwrt24"

cd imx_openwrt
cp ../variantfiles/configs/$config_file .config
make -j8
