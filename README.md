# arduino-latency-test
Measure "glass-to-glass" latency using Arduino, a LED and a photoresistor

Useful for measuring and comparing video latency. Film the LED and attach the photoresistor to the transmitted image of the LED on a screen. This will output the video latency to the serial console as well as display it on a attached OLED screen. (This part can be commented out if you don't have a screen available.)

### Adjusting the latency offset

Since the LED and the photoresistor tend to introduce their own latency, you can adjust the offset to compensate for it. This depends on the type of your LED and photoresistor. Test it by holding the photoresistor directly to the LED and adjusting the offset until you see a stable latency close to 0.

### Adjusting the LDR treshold

Currently, the latency is only measuren when the LED turns on to prevent measuring any afterglow, which might falsify the average reading. Pick a LDR threshold that sits in the middle of the measured minimum and maximum photoresistor readings. (The current reading is also output on the OLED screen to make adjusting this value easier.)


### Parts used

* Arduino UNO
* Any LED + corresponding resistor
* Any photoresistor + corresponding resistor
* AZOLED12864-5 OLED screen (not necesarry if reading out via serial)
