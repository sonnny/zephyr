// driver no header file just .c source

#define DT_DRV_COMPAT recto_sn754410

#include "recto_sn754410.h"

uint32_t period = PWM_HZ(1000); // 1,000,000 nanoseconds (1 ms)

// config file, this will hold info extracted from device tree
struct recto_sn754410_config {
  struct pwm_dt_spec en1_2; // speed control pwm
  struct pwm_dt_spec en3_4;
  struct gpio_dt_spec in1;  // digital control rotate/stop
  struct gpio_dt_spec in2;
  struct gpio_dt_spec in3;
  struct gpio_dt_spec in4;
  uint32_t id; };

// zephyr boot process will call this and fill in dev info
static int recto_sn754410_init(const struct device *dev){
  // todo error checking
  const struct recto_sn754410_config *cfg = dev->config;
  
  pwm_set_dt(&cfg->en1_2, period, (uint32_t)0);
  pwm_set_dt(&cfg->en3_4, period, (uint32_t)0);
  gpio_pin_configure_dt(&cfg->in1, GPIO_OUTPUT_INACTIVE);
  gpio_pin_configure_dt(&cfg->in2, GPIO_OUTPUT_INACTIVE);
  gpio_pin_configure_dt(&cfg->in3, GPIO_OUTPUT_INACTIVE);
  gpio_pin_configure_dt(&cfg->in4, GPIO_OUTPUT_INACTIVE);
  return 0;}


// turn motor clockwise
//    param: device, which motor 1 or 2, cw or ccw true of false
static int m_clockwise(const struct device *dev, int n, bool clockwise ){
  const struct recto_sn754410_config *cfg = dev->config;
  
  switch(n){
  
    case 1: if(clockwise){
              gpio_pin_set_dt(&cfg->in1, 0);
              gpio_pin_set_dt(&cfg->in2, 1);
            }
            else {
              gpio_pin_set_dt(&cfg->in1, 1);
              gpio_pin_set_dt(&cfg->in2, 0);
            }
            break;
            
    case 2: if(clockwise){
              gpio_pin_set_dt(&cfg->in3, 0);
              gpio_pin_set_dt(&cfg->in4, 1);
            }
            else {
              gpio_pin_set_dt(&cfg->in3, 1);
              gpio_pin_set_dt(&cfg->in4, 0);
            }
            break;
  }
  return 0;}

static int m_speed(const struct device *dev, int n, char percent){

  const struct recto_sn754410_config *cfg = dev->config;
  
  uint32_t pulse = period / (uint32_t)percent;
  
  switch(n){
  
    case 1: pwm_set_dt(&cfg->en1_2, period, pulse);
            break;
            
    case 2: pwm_set_dt(&cfg->en3_4, period, pulse);
            break;
  }
  return 0;}

static int m_stop(const struct device *dev, int n){

  const struct recto_sn754410_config *cfg = dev->config;
  
   switch(n){
  
    case 1: pwm_set_dt(&cfg->en1_2, period, 0);
            break;
            
    case 2: pwm_set_dt(&cfg->en3_4, period, 0);
            break;
  }
  return 0;}

// assigned user function to this driver function
// zephyr will use this in instance creation below
static const struct recto_sn754410_api motor_api_funcs = {
  .motors_clockwise = m_clockwise,
  .motors_speed = m_speed,
  .motors_stop = m_stop, };
  
 #define RECTO_SN754410_DEFINE(inst)                            \
                                                              \
    /* Create an instance of the config struct, populate with DT values */  \
    static struct recto_sn754410_config motor_cfg_##inst = {    \
        .en1_2 = PWM_DT_SPEC_INST_GET_BY_IDX(inst, 0),        \
        .en3_4 = PWM_DT_SPEC_INST_GET_BY_IDX(inst, 1),        \
        .in1 = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 0),  \
        .in2 = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 1),  \
        .in3 = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 2),  \
        .in4 = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 3),  \
        .id = inst                                                          \
    };                                                                      \
                                                                            \
    /* Create a "device" instance from a Devicetree node identifier and */  \
    /* registers the init function to run during boot. */                   \
    DEVICE_DT_INST_DEFINE(inst,                                             \
          recto_sn754410_init,         \
          NULL,                        \
          NULL,                        \
          &motor_cfg_##inst,           \
          POST_KERNEL,                 \
          CONFIG_PWM_INIT_PRIORITY,    \
          &motor_api_funcs);           \

DT_INST_FOREACH_STATUS_OKAY(RECTO_SN754410_DEFINE)


