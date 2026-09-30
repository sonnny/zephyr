// since the original compatible is "simple,leds"
// we cannot use comma in the macro name and need
// to be replace with underscore
// this macro connects the driver source file with
// the device tree nodes that declare
// from this point driver macros such as DT_INST_* will
// automatically match instances of the compatible and
// generate the necessary device struct for each one
#define DT_DRV_COMPAT simple_leds

#include "simple_leds.h"

// Configuration structure
// this will hold typically the info extracted from the device tree
//     like gpio specifications
struct simple_leds_config {
    struct gpio_dt_spec led;
    uint32_t id;
};


// init function below accept a pointer to a struct device
// as required by zephyr driver model
// inside the function, cast the device config to
// your simple_leds_config structure so you can access
// the gpio_dt_spec and configure the gpio properly
// there is also no prototye in simple_leds.h for init function below
//   it is meant to remain private to the driver
// there is no need to call init manually since it is
// automatically executed during the zephyr boot process as
// part of the device initialization sequence
static int simple_leds_init(const struct device *dev)
{
    const struct simple_leds_config *cfg = dev->config; 
    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);

    return 0;
}

// by declaring driver functions below static
// this makes them private to the driver
// from now on these internal functions will not be
// called directly by the app instead will only
// use static inline wrapper functions defined in simple_leds.h
// which act as public interface to the driver
static int leds_off( const struct device *dev )
{
    const struct simple_leds_config *cfg = dev->config; 
    gpio_pin_set_dt(&cfg->led, 0);
    return 0;
}

/*just flip a given pin value*/
static int leds_toggle( const struct device *dev )
{
    const struct simple_leds_config *cfg = dev->config; 
    gpio_pin_toggle_dt(&cfg->led);
    return 0;
}

// new instance of struct simple_leds_api below
// will initialize the driver functions
// this must be declared after all the functions are
// defined, otherwise you will get an error that
//   the functions are missing
//
// struct simple_leds_api definition is in simple_leds.h file
static const struct simple_leds_api leds_api_funcs = {
    .off = leds_off,      /* this must match your existing function in this file */
    .toggle = leds_toggle /* this must match your existing function in this file */
};

// define below must be after driver functions
// and define a macro that registers the device so
// it is initalized during the zephyr boot process
#define SIMPLE_LEDS_DEFINE(inst)                                            \
                                                                            \
    /* Create an instance of the config struct, populate with DT values */  \
    static struct simple_leds_config leds_cfg_##inst = {                    \
        .led = GPIO_DT_SPEC_GET(DT_INST(inst, simple_leds), gpios),         \
        .id = inst                                                          \
    };                                                                      \
                                                                            \
    /* Create a "device" instance from a Devicetree node identifier and */  \
    /* registers the init function to run during boot. */                   \
    DEVICE_DT_INST_DEFINE(inst,                                             \
                          simple_leds_init,                                 \
                          NULL,                                             \
                          NULL,                                             \
                          &leds_cfg_##inst,                                 \
                          POST_KERNEL,                                      \
                          CONFIG_GPIO_INIT_PRIORITY,                        \
                          &leds_api_funcs);                                            \

DT_INST_FOREACH_STATUS_OKAY(SIMPLE_LEDS_DEFINE)
