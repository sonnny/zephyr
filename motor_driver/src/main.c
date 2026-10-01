/*
  build for rpi_pico only
  when i tried for maker_pi_rp2040
  i'm getting error on the overlay file even though it's for the same overlay
*/

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include "simple_motor.h"

const struct device *motor_dev = DEVICE_DT_GET( DT_NODELABEL(motor) ); 

int main(void)
{   
    simple_motor_stop(motor_dev);
    while(true)
    {   
       simple_motor_forward(motor_dev);
       for(int i=0; i<10; i++) {simple_motor_set_speed(motor_dev, i); k_msleep(500);}

       k_msleep(500);
       simple_motor_reverse(motor_dev);
       simple_motor_set_speed(motor_dev, 5);//50 percent
       k_msleep(3000);       
       simple_motor_stop(motor_dev);
       simple_motor_set_speed(motor_dev, 1);
       k_msleep(3000); 
    }
        
	return 0;
}
