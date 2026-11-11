/*
 * demo received dcc signal
 *
 */
 #include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <hardware/pio.h>
#include "dcc.c"

int main(void){
  uint8_t packet[8];
  pio_init();
 
   while(1) {
     k_msleep(1000);
     if (dcc_receiver_available()) {
       dcc_receiver_read(packet);
       for(int i=0; i<8; i++) printk(" %x\n",packet[i]);
       printk("\n");
     }
   }
 
 return 0;
 }
