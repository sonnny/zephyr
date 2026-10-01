#ifndef __ZEPHYR_SIMPLE_MOTOR_H__
#define __ZEPHYR_SIMPLE_MOTOR_H__

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

struct simple_motor_api {
    int (*stop)(const struct device *dev);
    int (*forward)(const struct device *dev);
    int (*reverse)(const struct device *dev);
    int (*set_speed)(const struct device *dev, char percent);
};

static inline int simple_motor_stop( const struct device *dev )
{
    const struct simple_motor_api *api = dev->api;
    return api->stop( dev );
}

static inline int simple_motor_forward( const struct device *dev )
{
    const struct simple_motor_api *api = dev->api;
    return api->forward( dev );
}

static inline int simple_motor_reverse( const struct device *dev )
{
    const struct simple_motor_api *api = dev->api;
    return api->reverse( dev );
}

static inline int simple_motor_set_speed( const struct device *dev, char percent )
{
    const struct simple_motor_api *api = dev->api;
    return api->set_speed( dev, percent );
}



#endif
