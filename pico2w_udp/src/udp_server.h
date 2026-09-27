/* https://c9sys.com/zephyr-esp32-wifi-udp-communication-en/  */

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>

#include <errno.h>
#include <string.h>

#include "ssd1306.h"
#include "motor.h"

#define UDP_LOCAL_PORT    8001
#define UDP_RX_STACK_SIZE 2048
#define UDP_RX_PRIORITY      5

K_THREAD_STACK_DEFINE(udp_rx_stack, UDP_RX_STACK_SIZE);

static struct k_thread udp_rx_thread_data;
static k_tid_t udp_rx_tid;

static void udp_rx_thread(void *p1, void *p2, void *p3){

  int sock;
  int ret;
  
  struct sockaddr_in local_addr;
  struct sockaddr_in remote_addr;
  
  socklen_t remote_addr_len;
  
  char rx_buffer[128];
  
  ARG_UNUSED(p1);
  ARG_UNUSED(p2);
  ARG_UNUSED(p3);
  
  /* create udp socket */
  sock =zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  
  if (sock < 0){
    printk("udp rx socket creation failed: %d\n", errno);
    return;
  }
  
  memset(&local_addr, 0, sizeof(local_addr));
  
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(UDP_LOCAL_PORT);
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  /* bind socket to local udp port */
  ret = zsock_bind(sock,
                   (struct sockaddr *)&local_addr,
                   sizeof(local_addr));
                   
  if (ret < 0){
    printk("udp rx bind failed: %d\n", errno);
    zsock_close(sock);
    return;
  }
  
  printk("udp rx waiting on port %d...\n", UDP_LOCAL_PORT);
  
  while (1){
    remote_addr_len = sizeof(remote_addr);
    
    ret = zsock_recvfrom(sock,
                         rx_buffer,
                         sizeof(rx_buffer) - 1,
                         0,
                         (struct sockaddr *)&remote_addr,
                         &remote_addr_len);
                         
    if (ret < 0){
      printk("udp rx receive failed: %d\n", errno);
      continue;
    }
    
    rx_buffer[ret] = '\0';
    printk("udp rx received: %s\n", rx_buffer);
    
    if (strcmp(rx_buffer, "stop") == 0) { motor_stop(); }
    else if (strcmp(rx_buffer, "left") == 0) { motor_left(); }
    else if (strcmp(rx_buffer, "right") == 0) { motor_right(); }
    else if (strcmp(rx_buffer, "up") == 0) { motor_up(); }
    else if (strcmp(rx_buffer, "down") == 0) { motor_down(); }
    
    ssd1306_set_text(rx_buffer, 0, 0);
  }
}

/* start upd receive thread */
void udp_server_start(void){

  udp_rx_tid = k_thread_create(&udp_rx_thread_data,
                               udp_rx_stack,
                               K_THREAD_STACK_SIZEOF(udp_rx_stack),
                               udp_rx_thread,
                               NULL,
                               NULL,
                               NULL,
                               UDP_RX_PRIORITY,
                               0,
                               K_NO_WAIT);
                               
  printk("udp rx thread started.\n");
}
  
