# Xilinx Virtual Cable Server for WR node management board
This tool is based on https://github.com/derekmulcahy/xvcpi


## Usage
* run built xvc application on the target board
* open Vivado tcl console and type:
```commandline
open_hw
connect_hw_server
open_hw_target -xvc_url <xvc-server>:2542
```
