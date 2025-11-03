cmake_minimum_required(VERSION 3.10)

# Set the project name
project(xvc C)

# Set the cross-compilation toolchain file
include(${CMAKE_SOURCE_DIR}/toolchain.cmake)

# # Find necessary libraries (e.g., pthread)
# find_package(Threads REQUIRED)

# Find the libgpiod library.
# CMake will look in ${CMAKE_FIND_ROOT_PATH}/usr/lib
find_library(GPIOD_LIBRARY NAMES gpiod)

# Find the gpiod header files.
# CMake will look in ${CMAKE_FIND_ROOT_PATH}/usr/include
find_path(GPIOD_INCLUDE_DIR NAMES gpiod.h)

# Check if the library and headers were found.
if(NOT GPIOD_LIBRARY OR NOT GPIOD_INCLUDE_DIR)
    message(FATAL_ERROR "libgpiod not found! Check your OpenWrt SDK path.")
endif()

add_executable(xvc main.c)

# Add the include directory for the gpiod headers
target_include_directories(xvc PRIVATE ${GPIOD_INCLUDE_DIR})

# Link the gpiod library to your executable
target_link_libraries(xvc PRIVATE ${GPIOD_LIBRARY})

# # Link the required libraries to the executable
# target_link_libraries(gpio_test PRIVATE Threads::Threads)

# Install the executable
install(TARGETS xvc DESTINATION /home/prasinski/nfs/root)