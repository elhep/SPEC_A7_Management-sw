set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

#set(CMAKE_STAGING_PREFIX /home/devel/stage)
# set STAGING_DIR="../imx_openwrt/staging_dir/"

set(CMAKE_FIND_ROOT_PATH "../imx_openwrt/staging_dir/target-aarch64_cortex-a53_musl")

set(tools ../imx_openwrt/staging_dir/toolchain-aarch64_cortex-a53_gcc-13.3.0_musl)
set(CMAKE_C_COMPILER ${tools}/bin/aarch64-openwrt-linux-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/aarch64-openwrt-linux-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

