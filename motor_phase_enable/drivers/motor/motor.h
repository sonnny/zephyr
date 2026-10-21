#ifndef __ZEPHYR_MOTOR_H__
#define __ZEPHYR_MOTOR_H__

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

struct motor_api {
    int (*rotate)(const struct device *dev, bool cw);
    int (*speed)(const struct device *dev, uint8_t percent);};

static inline int motor_rotate( const struct device *dev, bool cw ){
  const struct motor_api *api = dev->api;
  return api->rotate(dev, cw);}

static inline int motor_speed( const struct device *dev, uint8_t percent ){
  const struct motor_api *api = dev->api;
  return api->speed(dev, percent);}

#endif
