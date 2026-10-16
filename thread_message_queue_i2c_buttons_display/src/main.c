
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>

/* define mutex for i2c bus, we have several i2c slave to write/read to */
K_MUTEX_DEFINE(i2c_bus_mutex);

/* define message queue to share between threads */
#include "msg_queue.h"
K_MSGQ_DEFINE(my_shared_msgq, sizeof(struct my_message_t), 20, 4);

#include "display.h"
#include "buttons.c"
#include "digi_pot.h"
#include "buzzer.h"

int main(void){

  /* buttons init will setup callback */
  buttons_init();
  
  while (1) {
  
    k_msleep(2000);

  }
return 0;
}


