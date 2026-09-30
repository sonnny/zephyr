# Check if LEDS is set in Kconfig
if(CONFIG_SIMPLE_LEDS)
    # Add your source file directory
    zephyr_include_directories(.)

    # Add the source file you want to compile
    zephyr_library_sources(simple_leds.c)
endif()
