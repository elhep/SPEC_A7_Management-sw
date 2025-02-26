#!/bin/sh

# This script has to be run just after booting sequence to set
# initial configuration of the system:
# - config file management
# - gpio direction,
# - i2c mux configuration, etc.)

# copy status files to the RAM (/dev/shm/.)
mkdir /dev/shm/wrnode_status
cp -R /root/wrnode_status_init/. /dev/shm/wrnode_status
