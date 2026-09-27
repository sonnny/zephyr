cmake_minimum_required(VERSION 3.28.0)
set(BOARD rpi_pico2/rp2350a/m33/w)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wifi_connect)
target_sources(app PRIVATE src/main.c)


