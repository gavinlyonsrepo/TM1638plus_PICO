/*
* Project Name: TM1638plus 
* File: TM1638plus.h
* Description: TM1638plus.h header file arduino library for TM1638 module(LED & KEY). Model 1 & Model 3
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/TM1638plus_PICO
*/


#ifndef TM1638PLUS_H
#define TM1638PLUS_H

#include "pico/stdlib.h"
#include <cstdio>
#include <cstring>
#include "tm1638plus_common.h"

class TM1638plus : public TM1638plus_common  {

public:
	// Constructor 
	//Parameters 
	// 1. strobe = GPIO STB pin
	// 2. clock = GPIO CLK  pin
	// 3. data = GPIO DIO pin
	TM1638plus(uint8_t strobe, uint8_t clock, uint8_t data);
	
	// Methods

	//Read buttons returns a byte with value of buttons 1-8 b7b6b5b4b3b2b1b0
	// 1 pressed, zero not pressed. 
	//User may have to debounce buttons depending on application.
	//See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
	// for de-bonce example arduino
	uint8_t readButtons(void);

	// Send Text to Seven segments, passed char array pointer
	// dots are removed from string and dot on preceding digit switched on
	// "abc.def" will be shown as "abcdef" with c decimal point turned on.
	void displayText(const char *text);

	// Send ASCII value to seven segment, pass position 0-7 and ASCII value byte
	void displayASCII(uint8_t position, uint8_t ascii);

	// Same as displayASCII function but turns on dot/decimal point  as well 
	void displayASCIIwDot(uint8_t position, uint8_t ascii) ;

	// Send HEX value to seven segment, pass position 0-7 and hex value(DEC) 0-15
	void displayHex(uint8_t position, uint8_t hex);

	// Send seven segment value to seven segment
	//  pass position 0-7 byte of data corresponding to segments (dp)gfedcba
	// i.e 0b01000001 will set g and a on. 
	void display7Seg(uint8_t position, uint8_t value);
	
    // Display an integer and leading zeros optional
	// Param 1 :: integer to display 2^32 
	// Param 2 :: bool leading zeros , true on , false off
	// Param 3 :: enum text alignment , left or right alignment
	void displayIntNum(unsigned long number, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
	
	// Divides the display into two nibbles and displays a Decimal number in each.
	// takes in two numbers 0-9999 for each nibble ,
	// and leading zeros optional
	// Param 1 :: upper nibble integer 2^16 
	// Param 2 :: lower nibble integer 2^16 
	// Param 3 :: bool leading zeros , true on , false off
	// Param 4 :: enum text alignment , left or right alignment
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, bool leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
	
	  // Set the LEDs. passed one  16bit integer.
	  // MODEL 3:
	  //MSB byte for the green LEDs, LS byte for the red LEDs (0xgreenred) 
	  //ie. 0xE007   1110 0000 0000 0111   results in L8-L0  GGGX XRRR, NOTE L8 is RHS on display
	  // MODEL 1:
	  // MSB byte 1 for  red LED , LSB byte n/a set to 0x00 (0xleds, 0xXX)
	  //i.e 0xF100  1111 0000 L8-L0 RRRRXXX0 NOTE  L8 is RHS on display
    void setLEDs(uint16_t greenred);
    
    // Set an LED, pass it LED position 0-7 and value 0 or 1 , L1-L8
	void setLED(uint8_t position, uint8_t value);
	
};

#endif
