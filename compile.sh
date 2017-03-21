#!/bin/sh
source /opt/poky/2.2.1/environment-setup-cortexa9hf-neon-poky-linux-gnueabi
export LDFLAGS="" 
make distclean
make mx6qmarta_config
make