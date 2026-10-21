#define DT_DRV_COMPAT motor
#include "motor.h"

struct motor_config {
    struct pwm_dt_spec  enable;   
    struct gpio_dt_spec phase;};

static int motor_rotation(const struct device *dev, bool cw ){
  const struct motor_config *cfg = dev->config;
  if (cw) {
    gpio_pin_set_dt(&cfg->phase, 0);
  } else {
    gpio_pin_set_dt(&cfg->phase, 1);
  }
  return 0;} 

static int motor_set_speed(const struct device *dev, uint8_t percent){
   const struct motor_config *cfg = dev->config;
   uint32_t pulse = cfg->enable.period / 100 * percent;
   pwm_set_dt(&cfg->enable, cfg->enable.period, pulse);
   //uint32_t pulse = cfg->enable.period / 100 * percent;
   //pwm_set_pulse_dt(&cfg->enable, pulse);
   return 0;}

static int motor_init(const struct device *dev){
    const struct motor_config *cfg = dev->config; 
    /* todo should init pwm here */
    gpio_pin_configure_dt(&cfg->phase, GPIO_OUTPUT_INACTIVE);
    return 0;}

static const struct motor_api api = {
  .rotate = motor_rotation,
  .speed  = motor_set_speed,
};

#define MOTOR_DEFINE(inst)                                         \
  static struct motor_config cfg##inst = {                         \
    .enable = PWM_DT_SPEC_INST_GET_BY_IDX(inst, 0),                \
    .phase  = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios,0),};       \
                                                                   \
  DEVICE_DT_INST_DEFINE(inst, motor_init, NULL, NULL, &cfg##inst,  \
                    POST_KERNEL, CONFIG_PWM_INIT_PRIORITY, &api);  \

DT_INST_FOREACH_STATUS_OKAY(MOTOR_DEFINE)
