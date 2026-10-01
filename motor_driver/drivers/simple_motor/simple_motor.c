
#define DT_DRV_COMPAT simple_motor

#include "simple_motor.h"

struct simple_motor_config {
    struct pwm_dt_spec  en1;   
    struct gpio_dt_spec in1;
    struct gpio_dt_spec in2;  
    uint32_t id;
};

static int simple_motor_init(const struct device *dev)
{
    const struct simple_motor_config *cfg = dev->config; 
    
    uint32_t period = PWM_MSEC(1);
    uint32_t pulse = period / 2; // 50 %
    
    pwm_set_dt(&cfg->en1, period, pulse);
    gpio_pin_configure_dt(&cfg->in1, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&cfg->in2, GPIO_OUTPUT_INACTIVE);
    
    return 0;
}

static int motor_stop( const struct device *dev )
{
    const struct simple_motor_config *cfg = dev->config; 
    
    gpio_pin_set_dt(&cfg->in1, 0);
    gpio_pin_set_dt(&cfg->in2, 0); 
    return 0;
}

static int motor_forward( const struct device *dev )
{
    const struct simple_motor_config *cfg = dev->config;  
    
    gpio_pin_set_dt(&cfg->in1, 1);
    gpio_pin_set_dt(&cfg->in2, 0);    
    return 0;
}

static int motor_reverse( const struct device *dev )
{
    const struct simple_motor_config *cfg = dev->config;  
    gpio_pin_set_dt(&cfg->in1, 0);
    gpio_pin_set_dt(&cfg->in2, 1);    
    return 0;
}

static int motor_set_speed(const struct device *dev, char percent){
   const struct simple_motor_config *cfg = dev->config;
    
   uint32_t period = PWM_MSEC(1);
   uint32_t pulse = period / (uint32_t)percent;
   pwm_set_dt(&cfg->en1, period, pulse);
   return 0; }

/* struct simple_motor_api is in simple_motor.h */
static const struct simple_motor_api motor_api_funcs = {
    .stop = motor_stop,      /* this must match your existing function in this file */
    .forward = motor_forward,
    .reverse = motor_reverse,
    .set_speed = motor_set_speed,
};


#define SIMPLE_MOTOR_DEFINE(inst)                                            \
                                                                            \
    /* Create an instance of the config struct, populate with DT values */  \
    static struct simple_motor_config motor_cfg_##inst = {                    \
        .en1 = PWM_DT_SPEC_INST_GET_BY_IDX(inst, 0),         \
        .in1 = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 0),         \
        .in2 = GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 1),         \
        .id = inst                                                          \
    };                                                                      \
                                                                            \
    /* Create a "device" instance from a Devicetree node identifier and */  \
    /* registers the init function to run during boot. */                   \
    DEVICE_DT_INST_DEFINE(inst,                                             \
                          simple_motor_init,                                 \
                          NULL,                                             \
                          NULL,                                             \
                          &motor_cfg_##inst,                                 \
                          POST_KERNEL,                                      \
                          CONFIG_PWM_INIT_PRIORITY,                        \
                          &motor_api_funcs);                                            \

DT_INST_FOREACH_STATUS_OKAY(SIMPLE_MOTOR_DEFINE)

