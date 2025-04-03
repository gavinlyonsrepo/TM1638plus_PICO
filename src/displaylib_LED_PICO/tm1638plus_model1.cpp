/*!
	@file     tm1638plus_model1.cpp
	@author   Gavin Lyons
	@brief    PICO library source file for TM1638 module(LED & KEY). Model 1 
*/

#include "pico/stdlib.h"
#include "displaylib_LED_PICO/tm1638plus_model1.hpp"


/*!
	@brief Constructor for class TM1638plus
	@param strobe  GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
*/
TM1638plus_model1::TM1638plus_model1(uint8_t strobe, uint8_t clock, uint8_t data) : TM1638plus_common(strobe, clock, data) {
 // Blank constructor
}

/*!
	@brief Set ONE LED on or off  Model 1  & 3
	@param position  0-7  == L1-L8 on PCB
	@param  value  0 off 1 on
*/
void TM1638plus_model1::setLED(uint8_t position, uint8_t value)
{
	gpio_set_dir(_DATA_IO, GPIO_OUT);
	sendCommand(TM_WRITE_LOC);
	gpio_put(_STROBE_IO, false);
	sendData(TM_LEDS_ADR + (position << 1));
	sendData(value);
	gpio_put(_STROBE_IO, true);
}

/*!
	@brief Set all LED's  on or off  Model 1 & 3
	@param  ledvalues 1 on , 0 off , 0xXXLL where LL = L8-L1
	@note MODEL 1:
		-# Upper byte ignored this byte/method is used by sub-class Model 3 
		-# Lower byte LED data model 1
		-# setLEDs(0xF0) Displays as XXXX LLLL (L1-L8),NOTE on display L8 is on right hand side.
*/
void TM1638plus_model1::setLEDs(uint16_t ledvalues)
{
	uint8_t colour = 0;
	ledvalues = ledvalues & 0x00FF;
	for (uint8_t LEDposition = 0;  LEDposition < 8; LEDposition++) {
		if ((ledvalues & (1 << LEDposition)) != 0) {
			colour |= 0x01; 
		} 
		setLED(LEDposition, colour);
		colour = 0;
	}
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32
	@param TextAlignment  text alignment on display
*/
void TM1638plus_model1::displayIntNum(unsigned long number, TextAlignment_e TextAlignment)
{
	char values[TM_DISPLAY_SIZE + 1];
	const char* format;
	switch (TextAlignment)
	{
		case AlignLeft: format = "%ld";  break;
		case AlignRight: format = "%8ld"; break;
		case AlignRightZeros:format = "%08ld"; break;
		default: format = "%ld"; break;
	}
	// Format number into values buffer
	snprintf(values, TM_DISPLAY_SIZE + 1, format, number);
	displayText(values);
}

/*!
	@brief Display an integer in a nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param TextAlignment  text alignment on display
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void TM1638plus_model1::DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, TextAlignment_e TextAlignment)
{
	char valuesUpper[TM_DISPLAY_SIZE + 1];
	char valuesLower[TM_DISPLAY_SIZE / 2 + 1];

	// Select format based on alignment type
	const char* format;
	switch (TextAlignment)
	{
		case AlignLeft: format = "%-4d"; break;
		case AlignRight: format = "%4d"; break;
		case AlignRightZeros: format = "%04d"; break;
		default: format = "%4d"; break;
	}
	// Format numbers into buffers
	snprintf(valuesUpper, sizeof(valuesUpper), format, numberUpper);
	snprintf(valuesLower, sizeof(valuesLower), format, numberLower);
	strcat(valuesUpper, valuesLower);
	displayText(valuesUpper);
}


/*!
	@brief Display a text string  on display
	@param text    pointer to a character array
	@return Zero for success , -2 for null pointer
	@note 
		Dots are removed from string and dot on preceding digit switched on
		"abc.def" will be shown as "abcdef" with c decimal point turned on.
*/
int TM1638plus_model1::displayText(const char *text) {
	// Check for null pointer
	if (text == nullptr) 
	{
		printf("Error: displayText 1: String is a null pointer.\n");
		return -2;
	}
	char c, pos;
	pos = 0;
		while ((c = (*text++)) && pos < TM_DISPLAY_SIZE)  {
		if (*text == '.' && c != '.') {
			displayASCII(pos++, c, DecPointOn);

			text++;
		}  else {
			displayASCII(pos++, c, DecPointOff);
		}
		}
	return 0;
}


/*!
	@brief  Send seven segment value to seven segment
	@param position The position on display 0-7  
	@param value  byte of data corresponding to segments (dp)gfedcba 
	@note 	0b01000001 in value will set g and a on.
*/
void TM1638plus_model1::display7Seg(uint8_t position, uint8_t value) { // call 7-segment
	sendCommand(TM_WRITE_LOC);
	gpio_put(_STROBE_IO, false);
	sendData(TM_SEG_ADR + (position << 1));
	sendData(value);
	gpio_put(_STROBE_IO, true); 
}

/*!
	@brief Display an ASCII character on display
	@param position The position on display 0-7  
	@param ascii The ASCII value from font table  to display 
	@param decimalPoint decimal point or off on the digit.
*/
void TM1638plus_model1::displayASCII(uint8_t position, uint8_t ascii, DecimalPoint_e decimalPoint) {
	if (ascii < _ASCII_FONT_OFFSET || ascii >= _ASCII_FONT_END )
	{
		printf("Warning : displayASCII : ASCII character is outside font range %u, \n", ascii);
		ascii = '0';
	} // check ASCII font bounds
	const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
	switch (decimalPoint)
	{
		case DecPointOff: 
			display7Seg(position, font[ascii - _ASCII_FONT_OFFSET]); 
			break;
		case DecPointOn: // add 128 or 0x080 0b1000000 to turn on decimal point/dot in seven seg
			display7Seg(position, font[ascii- _ASCII_FONT_OFFSET] + DEC_POINT_7_MASK); 
			break;
	}
}

 /*!
	@brief  Send Hexadecimal value to seven segment
	@param position The position on display 0-7  
	@param hex  hexadecimal  value (DEC) 0-15  (0x00 - 0x0F)
*/
void TM1638plus_model1::displayHex(uint8_t position, uint8_t hex) 
{
		const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
		uint8_t offset = 0;
		hex = hex % 16;
		if (hex <= 9)
		{
				display7Seg(position, font[hex + _ASCII_FONT_HEX_OFFSET]);
				// 16 is offset in reduced ASCII table for 0 
		}else if ((hex >= 10) && (hex <=15))
		{
				// Calculate offset in reduced ASCII table for AbCDeF
				switch(hex) 
				{
				 case 10: offset = 'A'; break;
				 case 11: offset = 'b'; break;
				 case 12: offset = 'C'; break;
				 case 13: offset = 'd'; break;
				 case 14: offset = 'E'; break;
				 case 15: offset = 'F'; break;
				}
				display7Seg(position, font[offset-_ASCII_FONT_OFFSET]);
		}
}

/*!
	@brief  Read buttons values from display
	@return byte with value of buttons 1-8 b7b6b5b4b3b2b1b0 1 pressed, 0 not pressed.
	@note User may have to debounce buttons depending on application.
*/
uint8_t TM1638plus_model1::readButtons()
{
	uint8_t buttons = 0;
	uint8_t v =0;
	
	gpio_put(_STROBE_IO, false);
	sendData(TM_BUTTONS_MODE);
	gpio_set_dir(_DATA_IO, GPIO_IN);

	for (uint8_t i = 0; i < 4; i++)
	{
		v = HighFreqshiftin(_DATA_IO, _CLOCK_IO) << i;
		buttons |= v;
	}

	gpio_set_dir(_DATA_IO, GPIO_OUT);
	gpio_put(_STROBE_IO, true); 
	return buttons;
}

