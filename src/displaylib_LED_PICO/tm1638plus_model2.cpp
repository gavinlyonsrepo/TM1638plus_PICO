/*!
	@file   tm1638plus_model2.cpp
	@author Gavin Lyons
	@brief  PICO library,Source file for TM1638 module(16 KEY 16 pushbuttons. Model 2.
*/

#include "pico/stdlib.h"
#include "displaylib_LED_PICO/tm1638plus_model2.hpp" //include the library

/*!
	@brief Constructor for class TM1638plus_model2
	@param strobe GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
	@param  swap_nibbles default false, if true, swaps nibbles on display byte.
*/
TM1638plus_model2::TM1638plus_model2(uint8_t strobe, uint8_t clock, uint8_t data, bool swap_nibbles) : TM1638plus_common(strobe, clock, data)
{

	_SWAP_NIBBLES = swap_nibbles;
}

/*!
	@brief Send seven segment value to display
	@param segment 0-7 byte of data corresponding to segments abcdefg(dp) 01234567.
	@param digit  display digit position, 0x00 to 0xFF d8d7d6d5d54d3d2d1.
	@note
		for segment parameter a is 0 , dp is 7 , segment Value is which segments are off or on for each digit.
		To to set all "a" on send (0x00,0xFF). To set all segment "g" off (0x06,0X00)
*/
void TM1638plus_model2::DisplaySegments(uint8_t segment, uint8_t digit)
{
	if (_SWAP_NIBBLES == true)
	{
		uint8_t upper, lower = 0;
		lower = (digit) & 0x0F;      // select lower nibble
		upper = (digit >> 4) & 0X0F; // select upper nibble
		digit = lower << 4 | upper;
	}

	segment = (segment << 1);
	sendCommand(TM_WRITE_LOC);
	gpio_put(_STROBE_IO, false);
	sendData(TM_SEG_ADR | segment);
	sendData(digit);
	gpio_put(_STROBE_IO, true);
}

/*!
	@brief Display an Hexadecimal number  in each nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param dots Decimal point display, switch's on decimal point for those positions.  0 to 0xFF
	@param TextAlignment text alignment on display
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble. Converts to string internally.
*/
void TM1638plus_model2::DisplayHexNum(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, TextAlignment_e TextAlignment)
{
	char valuesUpper[TM_DISPLAY_SIZE / 2 + 1];
	char valuesLower[TM_DISPLAY_SIZE / 2 + 1];
	// Select format based on alignment type
	const char* format;
	switch (TextAlignment)
	{
		case AlignLeft: format = "%-4X"; break;
		case AlignRight: format = "%4X"; break;
		case AlignRightZeros: format = "%04X"; break;
		default: format = "%4X"; break;
	}
	// Format numbers into buffers
	snprintf(valuesUpper, sizeof(valuesUpper), format, numberUpper);
	snprintf(valuesLower, sizeof(valuesLower), format, numberLower);
	strcat(valuesUpper, valuesLower);
	DisplayStr(valuesUpper, dots);
}


/*!
	@brief Display an decimal number
	@param number  integer to display 2^32.
	@param dots Decimal point display, switch's on decimal point for those positions.
	@param TextAlignment text alignment on display.
	@note Converts to string internally
*/
void TM1638plus_model2::DisplayDecNum(unsigned long number, uint8_t dots, TextAlignment_e TextAlignment)
{
	char values[TM_DISPLAY_SIZE + 1];
	// Select format based on alignment type
	const char* format;
	switch (TextAlignment)
	{
		case AlignLeft: format = "%ld"; break;
		case AlignRight: format = "%8ld"; break;
		case AlignRightZeros: format = "%08ld"; break;
		default: format = "%ld"; break;
	}
	// Format number into values buffer
	snprintf(values, sizeof(values), format, number);
	DisplayStr(values, dots);
}


/*!
	@brief Display an integer in each nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param dots Turn on or off  decimal points  to 0xFF d7d6d5d4d3d2d1d0
	@param TextAlignment  text alignment on display
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void TM1638plus_model2::DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, TextAlignment_e TextAlignment)
{
	char valuesUpper[TM_DISPLAY_SIZE / 2 + 1];
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
	DisplayStr(valuesUpper, dots);
}

/*!
	@brief Display a string, with decimal point display
	@param string pointer to char array
	@param dots Turn on or off  decimal points 0 to 0xFF d7d6d5d4d3d2d1d0
	@note
		Takes in string , converts it to ASCII using the font and masks for the decimal point.
		Then passes array of eight ASCII bytes to DisplayValues function
		@return 0 for success, -2 for null ptr, -4 for ascii character outside font range
*/
int TM1638plus_model2::DisplayStr(const char *string, uint16_t dots)
{
	// Check for null pointer
	if (string== nullptr) 
	{
		printf("Error: DisplayStr 1: String is a null pointer.\n");
		return -2;
	}

	const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
	uint8_t values[TM_DISPLAY_SIZE];
	bool done = false;
	uint8_t Result = 0;
	memset(values, 0, TM_DISPLAY_SIZE * sizeof(uint8_t));

	for (uint8_t i = 0; i < TM_DISPLAY_SIZE; i++)
	{
		if (!done && string[i] != '\0')
		{
			if (string[i] < _ASCII_FONT_OFFSET || string[i] >=  _ASCII_FONT_END)
			{
				printf("Error: DisplayStr: Character '%c' (ASCII %u) is outside font range.\n", string[i], string[i]);
				return -4; // Error: Invalid character outside font range
			}
			if (dots >> (7 - i) & 1)
			{ // if dots bit is set for that position apply the mask to turn on dot(0x80).
				Result = font[string[i] - _ASCII_FONT_OFFSET];
				values[i] = (Result | DEC_POINT_7_MASK); // apply the Dot bitmask to value extracted from ASCII table
			}
			else
				values[i] = font[string[i] - _ASCII_FONT_OFFSET];
		}
		else
		{
			done = true;
			values[i] = (((dots >> (7 - i)) & 1) << 7);
		}
	}
	ASCIItoSegment(values);
	return 0;
}

/*!
	@brief Takes in Array of 8 ASCII bytes , Called from DisplayStr .
		 Scans each ASCII byte converts to array of 8 segment bytes where each byte represents a segment.
		 Then calls DisplaySegments() method to display segments on display
	@param values An array of 8 ASCII bytes
	@note
		byte 0 represents a in segment and then each bit represents the a segment in each digit.
		So for "00000005" is converted by DisplayStr to ASCII  hex"3F 3F 3F 3F 3F 3F 3F 6D" where left is first digit.
		this is converted to hex "FF FE FF FF FE FF 01 00" by ASCIItoSegment,  Where left is first segment.
		So "a" segment is turned on for all digits and "b" is on for all except last digit.
		The bits are  mapping below abcdefg(dp) = 01234567 ! .
		See for mapping of seven segment to digit https://en.wikipedia.org/wiki/Seven-segment_display
		We have to do this as TM1638 model 2 is addressed by segment not digit unlike Model 1&3
*/
void TM1638plus_model2::ASCIItoSegment(const uint8_t values[])
{
	for (uint8_t segment = 0; segment < TM_DISPLAY_SIZE; segment++)
	{
		uint8_t SegmentValue = 0;
		for (uint8_t j = 0; j < TM_DISPLAY_SIZE; j++)
		{
			SegmentValue |= ((values[j] >> segment) & 1) << (TM_DISPLAY_SIZE - j - 1);
		}
		DisplaySegments(segment, SegmentValue);
	}
}

/*!
	@brief Read push buttons method ( one of two methods available)
	@returns 0 if no button pressed or a  decimal value of buttons 1-16 (1 for S1 ... 16 for S16 ).
	@note
		 User may have to debounce buttons depending on application..
		 model 2 example here in ADC file TM1638plus_ADC_TEST_Model2.ino
*/
unsigned char TM1638plus_model2::ReadKey16()
{
	unsigned char c[4], i, key_value = 0;
	gpio_put(_STROBE_IO, false);
	sendData(TM_BUTTONS_MODE);
	gpio_set_dir(_DATA_IO, GPIO_IN);
	for (i = 0; i < 4; i++)
	{
		c[i] = HighFreqshiftin(_DATA_IO, _CLOCK_IO);
		if (c[i] == 0x04)
			key_value = 1 + (2 * i); // 00000100 4 0x04
		if (c[i] == 0x40)
			key_value = 2 + (2 * i); // 01000000 64 0x40
		if (c[i] == 0x02)
			key_value = 9 + (2 * i); // 00000010 2 0x02
		if (c[i] == 0x20)
			key_value = 10 + (2 * i); // 00100000 32 0x20
	}
	gpio_set_dir(_DATA_IO, GPIO_OUT);
	gpio_put(_STROBE_IO, true);
	return (key_value);
	// Data matrix for read key_value.
	// c3 0110 0110  c2 0110 0110  c1 0110 0110  c0 0110 0110 :bytes read
	//    8,16 7,15     6,14 5,13     4,12 3,11     2,10  1,9 :button value
}

/*!
	@brief Read push buttons method ( one of two methods available)
	@returns
		A 16 bit integer where each bit correspond to a switch.
		-# S1 = 0x0001
		-# S16 = 0x8000
		-# S1 + S16 together = 0x8001
	@note
		 Can detect multiple key presses. However,  See notes section in readme regarding,
		 problems with seven segment display when pressing certain keys in combination.
*/
uint16_t TM1638plus_model2::ReadKey16Two()
{

	uint16_t key_value = 0;
	uint8_t Datain, i = 0;
	gpio_put(_STROBE_IO, false);
	sendData(TM_BUTTONS_MODE);
	gpio_set_dir(_DATA_IO, GPIO_IN);
	for (i = 0; i < 4; i++)
	{
		Datain = HighFreqshiftin(_DATA_IO, _CLOCK_IO);
		// turn Datain ABCDEFGI = 0BC00FG0  into 00CG00BF see matrix below
		Datain = (((Datain & 0x40) >> 3 | (Datain & 0x04)) >> 2) | (Datain & 0x20) | (Datain & 0x02) << 3;
		// i = 0 Datain =  00,10,9,0021 // i = 1 Datain  = 00,12,11,0043
		// i = 2 Datain =  00 ,14,13,0065 // i = 3 Datain =  00,16,15,0087
		// key_value =  16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1.
		key_value |= ((Datain & 0x000F) << (2 * i)) | (((Datain & 0x00F0) << 4) << (2 * i));
	}
	gpio_set_dir(_DATA_IO, GPIO_OUT);
	gpio_put(_STROBE_IO, true);

	return (key_value);

	//  Data matrix for read key_value. c = datain
	//   c3 0110 0110  c2 0110 0110  c1 0110 0110  c0 0110 0110 :bytes read
	//    8,16 7,15     6,14 5,13     4,12 3,11      2,10  1,9 :button value
}
