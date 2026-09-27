#ifndef SSD1306_H
#define SSD1306_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>

    const struct device *display_dev;

int ssd1306_init(){
  int ret;
  
    /* Get the display device */
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        printk("Display device not ready!\n");
        return -1;
    }

    /* Initialize Character Framebuffer */
    ret = cfb_framebuffer_init(display_dev);
    if (ret) {
        printk("CFB init failed!\n");
        return -1;
    }

    /* Clear screen and turn on display */
    cfb_framebuffer_clear(display_dev, true);
    display_blanking_off(display_dev);

    /* Set font and print text */
    cfb_framebuffer_set_font(display_dev, 0);
    cfb_print(display_dev, "init...", 0, 0);

    /* Finalize the frame */
    cfb_framebuffer_finalize(display_dev);
    
    return 0;
}

void ssd1306_set_text(const char *const str, int16_t x, int16_t y){
     cfb_framebuffer_clear(display_dev, true);
     cfb_print(display_dev, str, x, y);
     cfb_framebuffer_finalize(display_dev);
}



#endif
