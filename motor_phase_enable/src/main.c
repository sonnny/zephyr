/*
 * demo for zephyr motor driver
 *   using phase and enable pins only
 *   like: cytron md13s
 *
 * motor_speed second argument is percentange 0 - 100
 *   depending on the motor and battery you may have to
 *   adjust the percentage before the motor start turning
 *
 * motor_rotate takes a boolean for second argument
 *   true to turn one way
 *   false to turn the opposite way
 *
 * to stop the motor just send zero percent to motor speed
 * see overlay file to see pin assignments
 * tested on an rp2040
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include "motor.h"

const struct device *motor = DEVICE_DT_GET(DT_NODELABEL(mymotor)); 

int main(void){
  motor_speed(motor, 60); // 60 lowest to turn on test motor
  while(true){
    motor_rotate(motor, true); // clockwise
    k_msleep(2000);
    motor_rotate(motor, false); // counter-clockwise
    k_msleep(2000);} 
 return 0;}
