# displaylib_LED_PICO README

## Overview

* Name: displaylib_LED_PICO
* Author: Gavin Lyons.
* Description:

C++ SDK Library to support LED segment display modules
for the Raspberry PI PICO
	
* Developed on Toolchain:
	1. Raspberry pi PICO RP2040
	2. SDK(1.4.0) C++20
	3. compiler G++ for arm-none-eabi((15:10.3-2021.07-4) 
	4. CMAKE(VERSION 3.18) , VScode(1.84.2)
	5. Linux Mint 22.1
	

* [URL project github link](https://github.com/gavinlyonsrepo/displaylib_LED_PICO)

* Donations welcome: [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Documentation

### Supported devices

| Component name | Type | Interface | Readme URL link |
| -------- | ---------- | --------- | ---------- |
| TM1637 |7 segment LED display module|GPIO| [Readme](extra/doc/tm1637/README.md)|
| TM1638 |7 segment LED display module|GPIO| [Readme](extra/doc/tm1638/README.md)|
| MAX7219|7 segment LED display module|SPI| [Readme](extra/doc/max7219/README.md)|
| HT16K33 |7,9,14 & 16 segment LED display module|I2C| [Readme](extra/doc/ht16k33/README.md)|

### API Documentation

The code is commented for doxygen and an application programming interface can be created using the doxygen software program.

## File system

Class Hierarchy:

![image link](https://github.com/gavinlyonsrepo/displaylib_LED_PICO/blob/main/extra/image/filesystem.png)
