/*
 * display used st7789
 *   amazon ASIN B0HC38FLGL
 *
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <string.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <lvgl_zephyr.h>

#include "msg_queue.h"

#define LVGL_THREAD_STACK_SIZE 4096
K_THREAD_STACK_DEFINE(lvgl_thread_stack, LVGL_THREAD_STACK_SIZE);

int ret;
lv_obj_t *hello_world_label;
lv_obj_t *count_label;
char uptime_str[32];
const struct device *display_dev;

void display_refresh_thread(void *p1, void *p2, void *p3){

  struct my_message_t received_msg;
  int ret;
  
  ARG_UNUSED(p1);
  ARG_UNUSED(p2);
  ARG_UNUSED(p3);
   
  display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
  
  hello_world_label = lv_label_create(lv_screen_active());
  lv_obj_set_style_text_font(hello_world_label, &lv_font_montserrat_28, 0);
  lv_label_set_text(hello_world_label, "display init...");
  lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);
  
  count_label = lv_label_create(lv_screen_active());
  lv_obj_set_style_text_font(count_label, &lv_font_montserrat_28, 0);
  lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);
  
  lv_timer_handler();
  ret = display_blanking_off(display_dev);
 
  while(1){
      ret = k_msgq_get(&my_shared_msgq, &received_msg, K_FOREVER);
      //ret = k_msgq_get(&my_shared_msgq, &received_msg, K_NO_WAIT);      
      if (ret == 0) { // if ret == 0 there is message in the queue
        if (received_msg.id == 1) {
          lv_label_set_text(hello_world_label, received_msg.str);
        } else if (received_msg.id == 2) {
          lv_label_set_text(count_label, received_msg.str);
        }
      } 
      
      lv_timer_handler();
  }
}
K_THREAD_DEFINE(lvgl_thread_id, LVGL_THREAD_STACK_SIZE, display_refresh_thread, NULL, NULL, NULL, 7, 0, 0);

#endif


