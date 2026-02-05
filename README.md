# sht3x temperature reader

Implementation of sht3x sensor temperature reader c program.

It has two threads: 

 - Temperature reading thread. It tries gets a new temperature value using the sht3x module, via i2c evry millisecond. The sensor needs 20ms to be read so we will have a new value every 21ms aprox.
 - Communication thread. gets the temperature from the temperature reading thread when the temperature request command is received, and sends it via UDP.

It works in imxrt1050-evkb and beaglebone. The development was started in the iMXRT1050 with FreeRTOS and conmtinued in a beglebone with linux. It should work in both platforms by modifying the pl_config.h and pl_hw_config.c/h files. This way the needed code for the different platforms is enabled or disabled if the portability layer files, and the same app code should work in both platforms.
