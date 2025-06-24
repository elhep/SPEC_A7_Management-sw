./dt-make-bin.sh imx93-qs93-eqos-lan8710.dts imx93-qs93-eqos-lan8710.dtb
./dt-make-bin.sh imx93-qs93-fec-lan8710.dts imx93-qs93-fec-lan8710.dtb
./dt-make-bin.sh imx93-qs93-spidev.dts imx93-qs93-spidev.dtb

fdtoverlay -i imx93-qs93-5210.dtb -o imx93-final.dtb \
imx93-qs93-eqos-lan8710.dtb \
imx93-qs93-fec-lan8710.dtb \
imx93-qs93-spidev.dtb
