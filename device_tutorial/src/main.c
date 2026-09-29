#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "simple_leds.h"

const struct device *led_dev0 = DEVICE_DT_GET( DT_NODELABEL(myled) ); 

int main(void)
{   
    while(true)
    {
        simple_leds_toggle(led_dev0);
        k_msleep(1000);
    }
        
	return 0;
}
