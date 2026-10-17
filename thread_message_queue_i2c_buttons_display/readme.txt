  my zephyr tutorial (work in progress)
  
  files:
  
    main.c
    msg_queue.h
    buttons.c
    buzzer.h
    digi_pot.h
    display.h
    
    app.overlay
    
  
    1) how to use st7789 lcd screen
       amazon ASIN B0HC38FLGL
       
    2) how to use parallax 5 point button
       https://reprap.org/wiki/Parallax_5_position_switch
       (up, down, left, right, center pulled up by 10k resistor)
       
    3) how to use zephyr message queue so that button can send message
       to display without worrying about synchronication
       
    4) how to start a thread by using macro K_THREAD_DEFINE
         this thread macro is used in display.h and digi_pot.h
    
    5) interface an i2c digital potentiometer from electric dollar store
         https://electricdollarstore.com/pot.html
    
    6) display button string in the middle of the display,
       display digital potentiometer in the bottom of the display
       it uses the msg id to know where to display:
         1 -- display in the middle
         2 -- display in the bottom
         
    7) regarding message queue
         digi_pot.h acts as producer
           send digital potentiometer values to the message queue
         buttons.c  acts as producer
           send which button pressed values to the message queue
         display.h  acts as consumer
           gets data from the queue
             if it's digital potentiometer value, display in the bottom
             if it's button press, display in the middle
         
    8) use several i2c slaves, implement mutex to lock i2c bus
         i2c digi_pot uses i2c mutex
           reads value using i2c
         i2c buzzer uses i2c mutex
           sends value using i2c
           
    9) i just reassign i2c1 pins to a more convenient location on my breadboard
         see app.overlay file
    
--------------------------------------------------------------------
troubleshooting:

  make sure message queue is define before first time use
    I was getting error using the queue and realize that the
      queue is not even create yet and I started using it

-------------------------------------------------------------------
device tree:

https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/weact/rp2350b_core/rp2350b_core-pinctrl.dtsi

https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/weact/rp2350b_core/rp2350b_core.dtsi

--------------------------------------------------------------------
  
  to build for we act rp2350b_core in $HOME/my-workspace/example-applications/app1 directory
  
  cd to $HOME/my-workspace/example-applications
  assumes there is an app.overlay and prj.conf in example-applications/app1 directory
  
  to build:
    west build -p -b rp2350b_core/rp2350b/m33_0 app1
    
  to flash using picoprobe:
    west flash --openocd /usr/local/bin/openocd --openocd-search /home/sonny/pico/openocd
    
  to flash using picoprobe adding openocd config in CMakeLists.txt
  add to CMakeList.txt above find_package
    set(OPENOCD /usr/local/bin/openocd)
    set(OPENOCD_DEFAULT_PATH /home/sonny/pico/openocd)
  then to flash just issue west flash 
  
  work in progress
  
  display st7789
    spi0_rx    -- gpio 16 not used
    spi0_csn   -- gpio 17
    spi0_sck   -- gpio 18
    spi0_tx    -- gpio 19
    dc         -- gpio 20
    reset      -- gpio 21
    blk tied high 3.3v for backlight
    
  buttons (parallax 5 way button pulled up 10k to 3.3v)
    left    -- gpio 24
    down    -- gpio 22
    right   -- gpio 14
    center  -- gpio 12
    up      -- gpio 10 
    
  i2c1 sda / scl pulled up by 10k resistor
    pinout from: https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/weact/rp2350b_core/rp2350b_core-pinctrl.dtsi
    
    line 28
    
    sda     -- gpio 6
    scl     -- gpio 7 
    

