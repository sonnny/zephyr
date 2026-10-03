/*
 * demo for motor controller sn754410 
 *
 * function call
 *
 *   motors_speed(motor device,
                  motor number, (1 or 2)
                  percent of speed (single digit i.e. 2 = 50%, 5 = 20 %_
                  )
                  
    motors_clockwise(motor device, motor number (1 or 2), true or false)
    
    motors_stop(motor device, motor number (1 or 2))
 */             

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include "recto_sn754410.h"

const struct device *motors = DEVICE_DT_GET( DT_NODELABEL(sn754410) ); 

int main(void)
{   

   if(!device_is_ready(motors)){
     printk("failed");
     return 0;}

  while(1){
  
    motors_speed(motors, 1, 2);         // motor 1 stop
    motors_speed(motors, 2, 2);         // motor 2 50% speed
    motors_clockwise(motors, 1, true);  // motor 1 clockwise
    motors_clockwise(motors, 2, false); // moto2 2 counter clockwise
    k_msleep(2000);

    motors_speed(motors, 1, 5); // full speed 100%
    motors_speed(motors, 2, 6);
    motors_clockwise(motors, 1, false);
    motors_clockwise(motors, 2, true);
    k_msleep(2000);
    
    motors_stop(motors, 1); // motor 1 stop
    motors_stop(motors, 2); // motor 2 stop
    k_msleep(2000);
  }
	return 0;
}
