# Minimum CMake required version
cmake_minimum_required( VERSION 3.20.0 )

# register new module and keep your app sources properly defined
# ZEPHYR_EXTRA_MODULES tells the build system to include your drivers/simple_leds
# directory as external zephyr module
set(ZEPHYR_EXTRA_MODULES "${CMAKE_SOURCE_DIR}/drivers/simple_leds")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Project and source files
project(demo)
target_sources(app PRIVATE src/main.c)

