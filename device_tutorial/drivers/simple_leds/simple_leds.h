#ifndef __ZEPHYR_SIMPLE_LEDS_H__
#define __ZEPHYR_SIMPLE_LEDS_H__

#include <zephyr/drivers/gpio.h>

// struct below expose the led control functions
// through the device driver interface
// it contains pointer to the functions the driver provides
// this struct define a new structure with function pointers
struct simple_leds_api {
    int (*off)(const struct device *dev);
    int (*toggle)(const struct device *dev);
};

// below functions are static inline wrapper functions that
// call the driver api through the device pointer
// this allows the compiler to optimize the calls since inline
// functions avoid additional function call overhead while
// hiding the internal api struct from the app code
static inline int  simple_leds_off( const struct device *dev )
{
    const struct simple_leds_api *api = dev->api;
    return api->off( dev );
}

static inline int  simple_leds_toggle( const struct device *dev )
{
    const struct simple_leds_api *api = dev->api;
    return api->toggle( dev );
}

#endif
