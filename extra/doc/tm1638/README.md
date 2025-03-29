# TM1638 README

## Table of contents

  * [Overview](#overview)
  * [Examples](#examples)
  * [Hardware](#hardware)
  * [See Also](#see-also)

## Overview

* Description:

A Raspberry pi PICO, C++ SDK, library to display data on a 8-digit TM1638 seven segment module.

## Examples

There are three examples files(main.cpp) in example folder, To select the one that builds 
comment it's path in, in the CMakeLists.txt file section add_executable section.
Comment one path in and one only.

|  List No | example file name  | Model Num | Desc|
| ------ | ------ |  ------ | ------ |
| 1 | MODEL1/main.cpp | 1 | test routines for model 1 |
| 2 | MODEL2/main.cpp | 2 | test routines for model 2 |
| 3 | MODEL3/main.cpp | 3 | test routines for model 3 |

## Hardware

Connections to RPI-PICO, pick any GPIO you want and change it the main.cpp:

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output
4. GND
5. VCC 3.3V ( If VCC is set to 5V, level shift GPIO to 3.3V or damage could result.)

This library supports three variants of the TM1638,
which for purposes of this documentation,
will be named Model 1 ,Model 2 and Model 3. 
Pictured below from left to right.

![ module pics ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)

| Model No | Module Name | LEDS | Push buttons |
| ------ | ------ |  ------ | ------ |
| 1 | TM1638 LED & KEY | 8 red only | 8 |
| 2 | TM1638 KEYS, QYF  | 0 | 16 |
| 3 | TM1638 V1.3 or LKM1638  | 8 bi color,  red and green  | 8 |

## See Also

This library is a port of my Arduino Library. There you will find the full documentation
including the API, which is near identical to PICO except ::
 
1. Extra examples files for Arduino and examples are .ino files.
2. High freq data member flag is not needed in PICO library.
3. PICO constructor has GPIO parameters in base class.
4. In file dependancy graphs ,High level file includes will differ. #include "pico/stdlib.h" will be missing etc.

[ Arduino github Link ](https://github.com/gavinlyonsrepo/TM1638plus)
