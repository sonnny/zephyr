#ifndef MOTOR_H
#define MOTOR_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec in1 = GPIO_DT_SPEC_GET(DT_ALIAS(in_1), gpios);
static const struct gpio_dt_spec in2 = GPIO_DT_SPEC_GET(DT_ALIAS(in_2), gpios);
static const struct gpio_dt_spec in3 = GPIO_DT_SPEC_GET(DT_ALIAS(in_3), gpios);
static const struct gpio_dt_spec in4 = GPIO_DT_SPEC_GET(DT_ALIAS(in_4), gpios);

int motor_init(){

  gpio_pin_configure_dt(&in1, GPIO_OUTPUT);
  gpio_pin_configure_dt(&in2, GPIO_OUTPUT);
  gpio_pin_configure_dt(&in3, GPIO_OUTPUT);
  gpio_pin_configure_dt(&in4, GPIO_OUTPUT);
  
  return 0;
}

void motor_stop(){
  gpio_pin_set_raw(in1.port, in1.pin, 0);
  gpio_pin_set_raw(in2.port, in2.pin, 0);
  gpio_pin_set_raw(in3.port, in3.pin, 0);
  gpio_pin_set_raw(in4.port, in4.pin, 0);
}

void motor_up(){
  gpio_pin_set_raw(in1.port, in1.pin, 1);
  gpio_pin_set_raw(in2.port, in2.pin, 0);
  gpio_pin_set_raw(in3.port, in3.pin, 0);
  gpio_pin_set_raw(in4.port, in4.pin, 1);
}

void motor_down(){
  gpio_pin_set_raw(in1.port, in1.pin, 0);
  gpio_pin_set_raw(in2.port, in2.pin, 1);
  gpio_pin_set_raw(in3.port, in3.pin, 1);
  gpio_pin_set_raw(in4.port, in4.pin, 0);
}

void motor_right(){
  gpio_pin_set_raw(in1.port, in1.pin, 1);
  gpio_pin_set_raw(in2.port, in2.pin, 0);
  gpio_pin_set_raw(in3.port, in3.pin, 1);
  gpio_pin_set_raw(in4.port, in4.pin, 0);
}

void motor_left(){
  gpio_pin_set_raw(in1.port, in1.pin, 0);
  gpio_pin_set_raw(in2.port, in2.pin, 1);
  gpio_pin_set_raw(in3.port, in3.pin, 0);
  gpio_pin_set_raw(in4.port, in4.pin, 1);
}




#endif
