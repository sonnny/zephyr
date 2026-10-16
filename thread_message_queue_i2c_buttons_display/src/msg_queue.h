#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <zephyr/kernel.h>

struct my_message_t {
  uint32_t id;
  char str[20];
};

/* declare the queue extern */
extern struct k_msgq my_shared_msgq;

#endif

  
