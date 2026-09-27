
tutorial for me of starting with zephyr
my os ubuntu

--------------------------------------------------

things I really liked about zephyr

1) Korean wifi tutorial is for esp32, I followed the tutorial
   for pico2w without any changes on the source code, amazing.
   
2) during build zephyr tells me certain memory is low setting
   and adjust it for me
   
3) build files to look at to see what it generated (see below things I learned)

4) just like #1 compiled blinky for lots of various board without source code change

5) build board name suggestions if board you're typing is wrong

----------------------------------------------------------------------

things I learned:

1) setting board in CMakeLists.txt
         I got tired of typing board name in build
            i.e. set(BOARD rpi_pico2/rp2350a/m33/w)
         
2) put 10k pull-up resistor for i2c data and clock lines
         this will save you hours of frustration
         
3) to view usb serial printk messages
         add this to build --> -S cdc-acm-console
         add this to prj.conf:
             CONFIG_PRINTK=y
             CONFIG_CONSOLE=y
             CONFIG_STDOUT_CONSOLE=y
         then I use --> tio /dev/ttyACM0
         
4) troubleshooting zephyr build generated dts file
         look in build/zephyr/zephyr.dts
         
5) troubleshooting something about error undefined reference to device dts ord some number
         look number in build/zephyr/include/generated/zephyr/devicetree_generated.h

6) zephyr cfb init is failing on build (character frame buffer for display)
         added to prj.conf
         CONFIG_HEAP_MEM_POOL_SIZE=16384
         
7) unable to use ssd1306 dts found in the internet
          added to build --> -DZEPHYR_SCA_VARIANT=dtdoctor
          tells me device enable but no drivers found
          after hours and hours of frustration it turns out that
          internet example --> compatible = "solomon,ssd1306fb"
            should be          compatible = "solomon,ssd1306" (fb deleted)
              really that's it, this probably is the most frustrating for me
              
8) the pico2w uses common dts with rpi_pico

9) don't rely solely on the internet, stale examples, make sure versions are updated
   make sure to look at the date of the search results

zephyr build for rpi_pico 2 w

zephyr board --  rpi_pico2/rp2350a/m33/w (set in CMakeLists.txt)

build commands:

     west blobs fetch hal_infineon
     
     west build -p -S cdc-acm-console . -DDTC_OVERLAY_FILE=pico2w.overlay
     
     west flash -r uf2
     
     
reference for wifi connect (Korean)
  https://c9sys.com/zephyr-esp32-wifi-ap-connection-en/
  
reference for udp server(Korean)
  https://c9sys.com/zephyr-esp32-wifi-udp-communication-en/
  

  

