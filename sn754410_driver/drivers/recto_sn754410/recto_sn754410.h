#ifndef RECTO_SN754410_H
#define RECTO_SN754410_H

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

struct recto_sn754410_api {
  int (*motors_clockwise)(const struct device *dev, int n, bool b);
  int (*motors_speed)(const struct device *dev, int n, char s);  
  int (*motors_stop)(const struct device *dev, int n);

};

// turn motor clocwise
//    need - device, which motor 1 or 2, true/false (cw or ccw)
static inline int motors_clockwise(const struct device *dev, int n, bool b){
  const struct recto_sn754410_api *api = dev->api;
  return api->motors_clockwise(dev, n, b);}
  
static inline int motors_speed(const struct device *dev, int n, char s){
  const struct recto_sn754410_api *api = dev->api;
  return api->motors_speed(dev, n, s);}
  
static inline int motors_stop(const struct device *dev, int n){
  const struct recto_sn754410_api *api = dev->api;
  return api->motors_stop(dev, n);}

  
#endif

