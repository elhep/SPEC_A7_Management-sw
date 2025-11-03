# this code is based on https://github.com/OnionIoT/c-cross-compile-example

# edit this line to set correct path to toolchain
CURR_DIR = $(shell pwd)
BUILDROOT_PATH = $(CURR_DIR)/../data/imx_openwrt

# # define the toolchain and target names
TOOLCHAIN_NAME = toolchain-aarch64_cortex-a53_gcc-13.3.0_musl
TARGET_NAME = target-aarch64_cortex-a53_musl

# define the relative paths
STAGING_DIR_RELATIVE = staging_dir
TOOLCHAIN_RELATIVE = $(STAGING_DIR_RELATIVE)/$(TOOLCHAIN_NAME)
TARGET_RELATIVE=$(STAGING_DIR_RELATIVE)/$(TARGET_NAME)

# define the toolchain paths
TOOLCHAIN = $(BUILDROOT_PATH)/$(TOOLCHAIN_RELATIVE)
TOOLCHAIN_BIN = $(BUILDROOT_PATH)/$(TOOLCHAIN_RELATIVE)/bin

TOOLCHAIN_INCLUDE = $(BUILDROOT_PATH)/$(TOOLCHAIN_RELATIVE)/include
TOOLCHAIN_LIB = $(BUILDROOT_PATH)/$(TOOLCHAIN_RELATIVE)/lib
TOOLCHAIN_USR_INCLUDE = $(BUILDROOT_PATH)/$(TOOLCHAIN_RELATIVE)/usr/include
TOOLCHAIN_USR_LIB = $(BUILDROOT_PATH)/$(TOOLCHAIN_RELATIVE)/usr/lib

# define the target paths
TARGET = $(BUILDROOT_PATH)/$(TARGET_RELATIVE)

TARGET_INCLUDE = $(BUILDROOT_PATH)/$(TARGET_RELATIVE)/include
TARGET_LIB = $(BUILDROOT_PATH)/$(TARGET_RELATIVE)/lib
TARGET_USR_INCLUDE = $(BUILDROOT_PATH)/$(TARGET_RELATIVE)/usr/include
TARGET_USR_LIB = $(BUILDROOT_PATH)/$(TARGET_RELATIVE)/usr/lib

export STAGING_DIR = $(BUILDROOT_PATH)/$(STAGING_DIR_RELATIVE)

# define the compilers and such
TOOLCHAIN_CC = $(TOOLCHAIN_BIN)/aarch64-openwrt-linux-gcc
TOOLCHAIN_CXX = $(TOOLCHAIN_BIN)/aarch64-openwrt-linux-g++
TOOLCHAIN_LD = $(TOOLCHAIN_BIN)/aarch64-openwrt-linux-ld

TOOLCHAIN_AR = $(TOOLCHAIN_BIN)/aarch64-openwrt-linux-ar
TOOLCHAIN_RANLIB = $(TOOLCHAIN_BIN)/aarch64-openwrt-linux-ranlib

# define the FLAGS
INCLUDE_LINES = -I $(TOOLCHAIN_USR_INCLUDE) -I $(TOOLCHAIN_INCLUDE) -I $(TARGET_USR_INCLUDE) -I $(TARGET_INCLUDE)
TOOLCHAIN_CFLAGS = -Os -pipe -fno-caller-saves -fno-plt -fhonour-copts -Wno-error=unused-but-set-variable -Wno-error=unused-result -Wformat -Werror=format-security -fstack-protector -D_FORTIFY_SOURCE=1 -Wl,-z,now -Wl,-z,relro
TOOLCHAIN_CFLAGS := $(TOOLCHAIN_CFLAGS) $(INCLUDE_LINES)

TOOLCHAIN_CXXFLAGS = $(TOOLCHAIN_CFLAGS)
TOOLCHAIN_CXXFLAGS = $(TOOLCHAIN_CXXFLAGS) $(INCLUDE_LINES)

TOOLCHAIN_LDFLAGS = -L$(TOOLCHAIN_USR_LIB) -L$(TOOLCHAIN_LIB) -L$(TARGET_USR_LIB) -L$(TARGET_LIB)

CC = $(TOOLCHAIN_CC)
CXX = $(TOOLCHAIN_CXX)
LD = $(TOOLCHAIN_LD)
CFLAGS = $(TOOLCHAIN_CFLAGS)
LDFLAGS = $(TOOLCHAIN_LDFLAGS)