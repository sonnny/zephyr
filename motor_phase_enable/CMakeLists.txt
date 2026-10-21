# Minimum CMake required version
cmake_minimum_required( VERSION 3.20.0 )

set(OPENOCD /usr/local/bin/openocd)
set(OPENOCD_DEFAULT_PATH /home/sonny/pico/openocd)

set(ZEPHYR_EXTRA_MODULES "${CMAKE_SOURCE_DIR}/drivers/motor")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Project and source files
project(demo)
target_sources(app PRIVATE src/main.c)

