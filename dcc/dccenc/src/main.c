/*
 * zephyr demo of nmra dcc generator
 * using rp2350b pio device
 *
 * motor controller
 *   cytron md13s pwm - tied to 3.3v
 *     dir - gpio 2
 *
 * i2c digital pot - electric dollar store
 * i2c two 7 segment led - electric dollar store
 *
 */
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include "dcc.c"

#define ADDRESS       20
#define PREAMBLE  0xFFFE
#define SPEED_128   0x3F

#define SEVEN_SEGMENT_ADDRESS 0x16
#define DIGIPOT_ADDRESS       0x28
#define DIGIPOT_REGISTER       200

const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));

uint64_t packet;
uint8_t bytes[8];
uint8_t checksum;
uint8_t speed;
uint8_t direction = 0;
uint8_t buf[2] = {0x02, 0x02};
int ret;

int main(void){
  pio_init();
  
  while(1){
  
    ret = i2c_reg_read_byte(i2c_dev, DIGIPOT_ADDRESS, DIGIPOT_REGISTER, &speed);
    buf[1] = speed;
    ret = i2c_write(i2c_dev, buf, sizeof(buf), SEVEN_SEGMENT_ADDRESS);
    
    if (speed > 100) {
      speed = speed - 100;
      direction = 1;}
    else {
      speed = abs(speed - 100);
      direction = 0;}
      
    if (direction) speed += 0x80;
    
    checksum = ADDRESS ^ SPEED_128 ^ speed;
    packet =  ((uint64_t)PREAMBLE<<48)
             |((uint64_t)ADDRESS<<40)
             |((uint64_t)SPEED_128<<31)
             |((uint64_t)speed<<22)
             |((uint64_t)checksum<<13)
             |((uint64_t)0x01<<12);

  for(int i=0; i< 8; i++) bytes[i] = (packet >> (i * 8)) & 0xFF;
  for(int i=7; i>0; i--) dcc_put8(bytes[i]);
  k_msleep(30);
  } return 0;}
  


