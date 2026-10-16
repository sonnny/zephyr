/*
 * i2c digital pot used
 *   https://electricdollarstore.com/pot.html
 *
 */

#ifndef __DIGI_POT_H
#define __DIGI_POT_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>

#include <stdio.h>
#include <string.h>

#include "msg_queue.h"

#define DIGIPOT_THREAD_STACK_SIZE 1024
K_THREAD_STACK_DEFINE(digipot_thread_stack, DIGIPOT_THREAD_STACK_SIZE);

const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));
#define ADDRESS 0x28
#define REGISTER_ADDRESS 0x64

void digipot_thread(void *p1, void *p2, void *p3){

  int ret;
  uint16_t result;
  uint8_t read_buf[1];
  
  while (1) {
  
    struct my_message_t msg = {.id = 2, .str[0] = '\0'};
  
    ret = i2c_reg_read_byte(dev, ADDRESS, REGISTER_ADDRESS, read_buf);
    
    /*
     * since we're reading register 100
     * return value in read_buf is 1 - 100 (see electricdollarstore.com)
     * then we multiple the return value by 10
     * then we convert the result to string and display to the lcd display
     */
    result = read_buf[0] * 10;
    sprintf(msg.str, "%d", result);
    
    if (k_msgq_put(&my_shared_msgq, &msg, K_NO_WAIT) != 0){
      k_msgq_purge(&my_shared_msgq);
    }
    
    k_msleep(50);
  }
}
K_THREAD_DEFINE(digipot_thread_id, DIGIPOT_THREAD_STACK_SIZE, digipot_thread, NULL, NULL, NULL, 6, 0, 0);

#endif
