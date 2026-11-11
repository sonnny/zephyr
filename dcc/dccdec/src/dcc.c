/*
 * ref: https://github.com/mikaelsundin/raspberrypi_rp2040_pio_dcc_receiver/tree/main
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/misc/pio_rpi_pico/pio_rpi_pico.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
 
RPI_PICO_PIO_DEFINE_PROGRAM(
  dcc_decoder,
  0,
  19,
           //     .wrap_target
    0xe02c, //  0: set    x, 12
    0x0023, //  1: jmp    !x, 3
    0x0043, //  2: jmp    x--, 3
    0x2020, //  3: wait   0 pin, 0
    0x2ea0, //  4: wait   1 pin, 0               [14]
    0x00c7, //  5: jmp    pin, 7
    0x0001, //  6: jmp    1
    0x0029, //  7: jmp    !x, 9
    0x0000, //  8: jmp    0
    0xe027, //  9: set    x, 7
    0x2020, // 10: wait   0 pin, 0
    0x2ea0, // 11: wait   1 pin, 0               [14]
    0x4001, // 12: in     pins, 1
    0x004a, // 13: jmp    x--, 10
    0xa0ce, // 14: mov    isr, ~isr
    0x8000, // 15: push   noblock
    0x2020, // 16: wait   0 pin, 0
    0x2ea0, // 17: wait   1 pin, 0               [14]
    0x00c9, // 18: jmp    pin, 9
    0xc030, // 19: irq    wait 0 rel
            //     .wrap
);

#define DCC_INPUT 2

PIO pio = pio0;
int sm;

void pio_init() {
  gpio_init(DCC_INPUT);
  gpio_set_pulls(DCC_INPUT, true, false);
  sm = pio_claim_unused_sm(pio, true);
  const struct pio_program *program = RPI_PICO_PIO_GET_PROGRAM(dcc_decoder);
  uint offset = pio_add_program(pio, program);
  pio_sm_config config = pio_get_default_sm_config();
  sm_config_set_wrap(
    &config,
    offset + RPI_PICO_PIO_GET_WRAP_TARGET(dcc_decoder),
    offset + RPI_PICO_PIO_GET_WRAP(dcc_decoder));
  sm_config_set_in_pins(&config, DCC_INPUT);
  sm_config_set_jmp_pin(&config, DCC_INPUT);
  sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_RX);
  sm_config_set_clkdiv(&config, 625);
  sm_config_set_in_shift(&config, false, false, 8);
  pio_sm_init(pio, sm, offset, &config);
  pio_sm_set_enabled(pio, sm, true);}
    
bool dcc_receiver_available() {
  return pio_interrupt_get(pio, sm); }
  
uint dcc_receiver_read(uint8_t packet[8]) {
  //check if we get data
  if (dcc_receiver_available(pio, sm) == false) return 0;
  uint length = pio_sm_get_rx_fifo_level(pio, sm);
  for(int i=0; i<8; i++) packet[i] = pio_sm_get(pio, sm);
  //clear flag
  pio_interrupt_clear(pio, sm);
  return length;}
