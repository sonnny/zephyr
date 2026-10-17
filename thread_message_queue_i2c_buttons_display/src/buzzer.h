/*
 * i2c buzzer used
 *   https://electricdollarstore.com/beep.html
 *
 */

#ifndef __BUZZER_H
#define __BUZZER_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

const struct device *dev_buzzer = DEVICE_DT_GET(DT_ALIAS(my_i2c1));
#define BUZZER_ADDRESS 0x30

void buzzer_thread(){

  /* 100 - millisecond duration, 100 - midi note */
  uint8_t buzzer_buf[2] = {100, 100};
  int ret;
  
  while (1) {
  
    /* lock i2c bus, some threads can write/read anytime */
    k_mutex_lock(&i2c_bus_mutex, K_MSEC(100));
    ret = i2c_write(dev_buzzer, buzzer_buf, sizeof(buzzer_buf), BUZZER_ADDRESS);
    k_mutex_unlock(&i2c_bus_mutex);
  
    k_msleep(2000);
  }
}

K_THREAD_DEFINE(buzzer_thread_id, 1024, buzzer_thread, NULL, NULL, NULL, 9, 0, 0);

#endif
