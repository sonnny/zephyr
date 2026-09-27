
/*
 * to build:
 *    west build -p -S cdc-acm-console . -DDTC_OVERLAY_FILE=pico2w.overlay
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "ssd1306.h"
#include "wifi_connect.h"
#include "udp_server.h"
#include "motor.h"
                              
int main(void){
  
  printk("starting ssd1306...");
  ssd1306_init();
  motor_init();
  
  printk("starting wifi connect ...");
  ssd1306_set_text("starting wifi connect", 0, 0);
  wifi_connect_init();
  udp_server_start();
  
  return 0;
}

