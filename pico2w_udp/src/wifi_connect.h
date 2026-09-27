
/* https://c9sys.com/zephyr-esp32-wifi-ap-connection-en/  */

#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/printk.h>

#include "ssd1306.h"

#define WIFI_SSID     "HOME_SSID"
#define WIFI_PASSWORD "HOME_SSID_PASSWORD"

static struct net_mgmt_event_callback wifi_connect_cb;
static struct net_mgmt_event_callback ipv4_cb;

static void wifi_connect_handler(struct net_mgmt_event_callback *cb,
                                 uint64_t mgmt_event,
                                 struct net_if *iface);
                                 
static void ipv4_addr_handler(struct net_mgmt_event_callback *cb,
                              uint64_t mgmt_event,
                              struct net_if *iface);
                              
int wifi_connect_init(){
  struct net_if *iface;
  struct wifi_connect_req_params params = {0};
  int ret;
  
  printk("pico2w wifi connect test \n");
  
  iface = net_if_get_default();
  
  if (iface == NULL){
    printk("no default network interface found.\n");
    return 0;
  }
  
  /* register wifi connect result callback */
  net_mgmt_init_event_callback(&wifi_connect_cb,
                               wifi_connect_handler,
                               NET_EVENT_WIFI_CONNECT_RESULT);
                               
  net_mgmt_add_event_callback(&wifi_connect_cb);
  
  /* register ipv4 address callback */
  net_mgmt_init_event_callback(&ipv4_cb,
                               ipv4_addr_handler,
                               NET_EVENT_IPV4_ADDR_ADD);
                               
  net_mgmt_add_event_callback(&ipv4_cb);
  
  /* wifi connect params */
  params.ssid = (const uint8_t *)WIFI_SSID;
  params.ssid_length = strlen(WIFI_SSID);
  
  params.psk = (const uint8_t *)WIFI_PASSWORD;
  params.psk_length = strlen(WIFI_PASSWORD);
  
  params.security = WIFI_SECURITY_TYPE_PSK;
  params.channel = WIFI_CHANNEL_ANY;
  
  printk("connecting to wifi ap: %s\n", WIFI_SSID);
  
  /* request wifi connection */
  ret = net_mgmt(NET_REQUEST_WIFI_CONNECT,
                 iface,
                 &params,
                 sizeof(params));
                 
  if (ret != 0){
    printk("wifi connect request failed: %d\n", ret);
  }
  
  return 0;
}

static void wifi_connect_handler(struct net_mgmt_event_callback *cb,
                                 uint64_t mgmt_event,
                                 struct net_if *iface){

  const struct wifi_status *status = (const struct wifi_status *)cb->info;
  
  ARG_UNUSED(mgmt_event);
  ARG_UNUSED(iface);
  
  if (status->status == 0){
    printk("wifi connected successfully.\n");
  } else {
    printk("wifi connection failed: %d\n",status->status);
  }
}

static void ipv4_addr_handler(struct net_mgmt_event_callback *cb,
                              uint64_t mgmt_event,
                              struct net_if *iface){
  struct net_in_addr *addr;
  char addr_str[NET_IPV4_ADDR_LEN];
  
  ARG_UNUSED(cb);
  ARG_UNUSED(mgmt_event);
  
  addr = net_if_ipv4_get_global_addr(iface, NET_ADDR_PREFERRED);
  
  if (addr == NULL){
    printk("ipv4 address not found.\n");
    return;
  }
  
  net_addr_ntop(AF_INET,
                addr,
                addr_str,
                sizeof(addr_str));
                
  /* printk("ipv4 address: %s\n", addr_str); */
  ssd1306_set_text(addr_str, 0, 0);
}

#endif
