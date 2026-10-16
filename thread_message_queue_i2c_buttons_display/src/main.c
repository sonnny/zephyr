
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>

#include "msg_queue.h"
/* 
 * init queue to share between threads
 * allow 20 messages
 * 4 is alignment
 */
K_MSGQ_DEFINE(my_shared_msgq, sizeof(struct my_message_t), 20, 4);

#include "display.h" /* init display and run display thread */
#include "buttons.c"
#include "digi_pot.h"

int main(void){

  /*
   * buttons_init() will setup a callback whenever
   * a button is pressed, no need to create a thread
   * to monitor buttons
   */
  buttons_init();
  
  while (1) {
  
    k_msleep(1000);

  }
return 0;
}


