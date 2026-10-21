if(CONFIG_MOTOR)
    zephyr_include_directories(.)
    zephyr_library_sources(motor.c)
endif()
