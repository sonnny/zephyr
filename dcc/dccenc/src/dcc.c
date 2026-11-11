#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/misc/pio_rpi_pico/pio_rpi_pico.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>

RPI_PICO_PIO_DEFINE_PROGRAM(
  dcc_pulse,
  0,
  7,
          //     .wrap_target
  0xf401, //  0: set    pins, 1                [20]
  0x6021, //  1: out    x, 1                       
  0x0025, //  2: jmp    !x, 5                      
  0xf500, //  3: set    pins, 0                [21]
  0x0000, //  4: jmp    0                          
  0xb042, //  5: nop                           [16]
  0xfe00, //  6: set    pins, 0                [30]
  0xa842  //  7: nop                           [8] 
          //     .wrap
);

#define DCC_PIN 2

PIO pio = pio0;
int sm;

void pio_init() {
  sm = pio_claim_unused_sm(pio, true);
  const struct pio_program *program = RPI_PICO_PIO_GET_PROGRAM(dcc_pulse);
  uint offset = pio_add_program(pio, program);
  pio_sm_config config = pio_get_default_sm_config();
  sm_config_set_wrap(
    &config,
    offset + RPI_PICO_PIO_GET_WRAP_TARGET(dcc_pulse),
    offset + RPI_PICO_PIO_GET_WRAP(dcc_pulse));
    
  pio_gpio_init(pio, DCC_PIN);
  pio_sm_set_consecutive_pindirs(pio, sm, DCC_PIN, 1, true);
  sm_config_set_set_pins(&config, DCC_PIN, 1);
  sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_TX);
  float div = clock_get_hz(clk_sys) / 400000.0;
  sm_config_set_clkdiv(&config, div);
  sm_config_set_out_shift(&config, false, true, 8);
  pio_sm_init(pio, sm, offset, &config);
  pio_sm_set_enabled(pio, sm, true); }
  
void dcc_put8(uint8_t data){
  io_rw_8 *txfifo = (io_rw_8 *)&pio->txf[sm];
  *txfifo = data;}
  

