# RaspberryPi1-FreeRTOSv9.0.0
FreeRTOS v9.0.0 port for Raspberry Pi 1

This port includes:
- FreeRTOS v9.0.0 and the required modification to run on Raspberry Pi 1
- BCM2835 library to interface most of the devices present in the BCM2835 chip (I2C, SPI, GPIOS...). This library was originally written for Linux, I adapted it to run in bare metal
- A driver to use the miniuart on GPIO pins 14 and 15

Credit to the following people:
- James Walmsley who made the original FreeRTOS v7.2.0 - https://github.com/jameswalmsley/RaspberryPi-FreeRTOS
- David Welch who wrote a lot of bare metal related stuff - https://github.com/dwelch67/raspberrypi
- Mike McCauley who wrote the BCM2835 library for Linux - http://www.airspayce.com/mikem/bcm2835/

Requires:
- arm-none-eabi Toolchain: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
- Edit file FreeRTOS/Demo/ARM6_BCM2835/MakefileRPI1.cfg to change TOOLCHAINROOT to the root of the toolchain

Todo list:
- Finish cleaning up BCM2835 lib
- Finish and test mini UART driver
- Use System Timer for FreeRTOS scheduling instead of ARM Timer