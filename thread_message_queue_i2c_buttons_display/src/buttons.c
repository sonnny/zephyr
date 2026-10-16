/*
 * https://reprap.org/wiki/Parallax_5_position_switch
 *
 */
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "msg_queue.h"

int ret;

#define SW_NODE_24 DT_ALIAS(sw24)
static const struct gpio_dt_spec sw24 = GPIO_DT_SPEC_GET(SW_NODE_24, gpios);

#define SW_NODE_22 DT_ALIAS(sw22)
static const struct gpio_dt_spec sw22 = GPIO_DT_SPEC_GET(SW_NODE_22, gpios);

#define SW_NODE_14 DT_ALIAS(sw14)
static const struct gpio_dt_spec sw14 = GPIO_DT_SPEC_GET(SW_NODE_14, gpios);

#define SW_NODE_12 DT_ALIAS(sw12)
static const struct gpio_dt_spec sw12 = GPIO_DT_SPEC_GET(SW_NODE_12, gpios);

#define SW_NODE_10 DT_ALIAS(sw10)
static const struct gpio_dt_spec sw10 = GPIO_DT_SPEC_GET(SW_NODE_10, gpios);

static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
  struct my_message_t msg = {.id = 1, .str[0] = '\0'};
  
  if (pins & BIT(24)) {
    strcpy(msg.str, "  LEFT   ");
    if (k_msgq_put(&my_shared_msgq, &msg, K_NO_WAIT) != 0){
      k_msgq_purge(&my_shared_msgq);
    }
  } else if (pins & BIT(22)) {
    strcpy(msg.str, "  DOWN  ");
    if (k_msgq_put(&my_shared_msgq, &msg, K_NO_WAIT) != 0){
      k_msgq_purge(&my_shared_msgq);
    }
  } else if (pins & BIT(14)) {
    strcpy(msg.str, "  RIGHT  ");
    if (k_msgq_put(&my_shared_msgq, &msg, K_NO_WAIT) != 0){
      k_msgq_purge(&my_shared_msgq);
    } 
  } else if (pins & BIT(12)) {
    strcpy(msg.str, "  CENTER  ");
    if (k_msgq_put(&my_shared_msgq, &msg, K_NO_WAIT) != 0){
      k_msgq_purge(&my_shared_msgq);
    }
  } else if (pins & BIT(10)) {
    strcpy(msg.str, "  UP  ");
    if (k_msgq_put(&my_shared_msgq, &msg, K_NO_WAIT) != 0){
      k_msgq_purge(&my_shared_msgq);
    }
  }
}

void buttons_init(){
  
  gpio_pin_configure_dt(&sw24, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&sw24, GPIO_INT_EDGE_TO_ACTIVE);

  gpio_pin_configure_dt(&sw22, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&sw22, GPIO_INT_EDGE_TO_ACTIVE); 
  
  gpio_pin_configure_dt(&sw14, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&sw14, GPIO_INT_EDGE_TO_ACTIVE);
  
  gpio_pin_configure_dt(&sw12, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&sw12, GPIO_INT_EDGE_TO_ACTIVE);
  
  gpio_pin_configure_dt(&sw10, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&sw10, GPIO_INT_EDGE_TO_ACTIVE);
  
  gpio_init_callback(&button_cb_data, button_pressed, BIT(sw24.pin) | BIT(sw22.pin) | BIT(sw14.pin) | BIT(sw12.pin) | BIT(sw10.pin));
  
  gpio_add_callback(sw24.port, &button_cb_data);
  gpio_add_callback(sw22.port, &button_cb_data);
  gpio_add_callback(sw14.port, &button_cb_data);
  gpio_add_callback(sw12.port, &button_cb_data);
  gpio_add_callback(sw10.port, &button_cb_data);
}
  
