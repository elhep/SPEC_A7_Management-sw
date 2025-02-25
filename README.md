# SPEC A7 Management Software
### Project description
This repository provides software for a plug-in board (with ARM processor, SFP and RJ45),
designed for WR node management.

The software is written for the Ka-Ro QS93-5210 (+ OpenWRT Linux):<br>
https://www.karo-electronics.de/qs93/

Hardware repository:<br>
https://github.com/elhep/SPEC_A7_Management


### Building OpenWRT (+ OpenWRT SDK)
```
$ cd qs93_linux
$ ./docker_build
$ ./docker_run

(in docker container):
$ ./download_openwrt.sh
$ ./build_openwrt.sh
```
built images can be found in:<br>
./qs93_linux/data/imx_openwrt/build_dir/target-aarch64_generic_musl/linux-imx_cortexa55

