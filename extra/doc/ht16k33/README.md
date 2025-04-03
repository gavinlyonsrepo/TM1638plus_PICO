# HT16K33 Readme

## Table of contents

  * [Overview](#overview)
  * [Hardware](#hardware)
	* [Seven segment](#seven-segment)
	* [Nine segment](#nine-segment)
	* [Fourteen segment](#fourteen-segment)
	* [Sixteen segment](#sixteen-segment)
  * [Software](#software)
	* [Example files](#example-files)
	* [I2C](#i2c)


## Overview

* Display Name:HT16K33
* Author: Gavin Lyons.
* Description:

A C++ SDK raspberry pi PICO library to display data on LED segment modules using the HT16K33 controller module.The Library supports 7, 9, 14, and 16 segment displays. The LED segment displays must be common cathode. At present the library does not support keyscan or some
 custom products on market(such as backpacks with semi-colons),
this is for the IC module pictured driving standard common cathode LED segments.

## Hardware

[![module ](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ht.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ht.jpg)

The HT16K33 is a RAM Mapping 16*8 LED Controller Driver with keyscan.
The 28 pin package can control up to 128 LEDS. In library use case that is 8 digits of an LED segment device.
Segments are connected to the A0-A15 on controller. Digits common are connected to C0-C7 on controller.
We cannot use the decimal point on 16 segment devices as controller
does not have enough control AX lines.
Max. 16 x 8 patterns, 16 segments and 8 commons.

 * 7 segment  = (7 segments + decimal point)  x 8 Digits = 64 LEDS
 * 9 segment  = (9 segments + decimal point)  X 8 Digits = 80 LEDS
 * 14 segment = (14 segments + decimal point) X 8 Digits = 120 LEDS
 * 16 segment = 16 Segments X 8 digits                   = 128 LEDS

[![segments](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/segment.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/segment.jpg)


### Seven segment

The seven segment example file was tested on and is set up for a 3 digit common
cathode LED display. LT0565GWK.

| HT16K33 | LED  Segment LT0565GWK|
| --- | --- |
| C0  | Com 12 DIG1 |
| C1  | Com 9 DIG2 |
| C2  | Com 8 DIG3 |
| A0  | A 11  |
| A1  | B 7 |
| A2  | C 4 |
| A3  | D 2 |
| A4  | E 1 |
| A5  | F 10 |
| A6  | G 5 |
| A7  | Dp 3 |

### Nine segment

No example file for this as did not have device. To change which segment display is enabled just pass
the relevant enum value as argument in the DisplayInit() function in setup.

### Fourteen segment

The Fourteen segment example file was tested on two digit common
cathode LED Display. LDD-F5406RI

| HT16K33 | LED Segment LDD-F5406RI |
| --- | --- |
| C0  | Com 16 DIG1 |
| C1  | Com 11 DIG2 |
| A0  | A 12  |
| A1  | B 10 |
| A2  | C 9 |
| A3  | D 7 |
| A4  | E 1 |
| A5  | F 18 |
| A6  | G1 13 |
| A7  | G2 6 |
| A8  | H 17 |
| A9  | J 15 |
| A10  | K 14 |
| A11 | L 5 |
| A12  | M 4 |
| A13  | N 2 |
| A14  | Dp 8 |

### Sixteen segment

No example file for this as did not have device. To change which segment display is enabled just pass
the relevant enum value as argument in the DisplayInit() function in setup. Decimal point not supported as is,
not enough segment lines.

## Software

### Example files

| Filepath | File Function |
| ---- | ---- |
| test_7_segment| Carries out test sequence testing 3 digit 7 segment |
| test_14_segment| Carries out test sequence testing 2 digit 14 segment |

### I2C

Hardware I2C.

 I2C-bus interface. I2C Connections to PICO in examples, user can pick 
 I2C0 or I2C1.

1. HT16K33 SCLK = I2C0 SCL 17
2. HT16K33 SDA =  I2C0 SDA 16

1. I2C Address is set by default to 0x70(your module could be different,
user can change argument passed into class constructor). The I2C address of module can be adjusted on PCB.

2. I2C Clock rate can be adjusted and different I2c interface can be selected I2C0 or I2C1

3. In the event of an error writing a byte, debug info with error code will be written to console.
Debug configuration flag(displaylib_LED_debug) must be set to true to see this output.
User can set:
Communications timeout in microseconds, the time that I2C read and writes function will wait for the entire transaction to complete.
Error timeout between retry attempts in event of an I2C error , in milliseconds.
Number of retry attempts in event of an I2C error.
Monitor the Error flag, Number of bytes written, or PICO_ERROR_GENERIC if address not acknowledged, no device present, or PICO_ERROR_TIMEOUT if a timeout occurred.
