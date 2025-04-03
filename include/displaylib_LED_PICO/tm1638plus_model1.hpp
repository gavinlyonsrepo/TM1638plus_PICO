/*!
	@file     tm1638plus_model1.hpp
	@author   Gavin Lyons
	@brief    PICO library Header file for TM1638 module(LED & KEY). Model 1 
*/

#ifndef TM1638PLUSMODEL1_H
#define TM1638PLUSMODEL1_H

#include "pico/stdlib.h"
#include <cstdio>
#include <cstring>
#include "tm1638plus_common.hpp"

/*!
	@brief Class for Model 1
*/
class TM1638plus_model1 : public TM1638plus_common  {

public:
	// Constructor 
	TM1638plus_model1(uint8_t strobe, uint8_t clock, uint8_t data);
	
	// Methods
	uint8_t readButtons(void);
	// LED methods defined as overridden in sub-class for Model 3
	virtual void setLEDs(uint16_t greenred);
	virtual void setLED(uint8_t position, uint8_t value);

	 int displayText(const char *text);
	void displayASCII(uint8_t position, uint8_t ascii, DecimalPoint_e decimalPoint= DecPointOff);
	void displayHex(uint8_t position, uint8_t hex);
	void display7Seg(uint8_t position, uint8_t value);
	void displayIntNum(unsigned long number, TextAlignment_e = AlignLeft);
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, TextAlignment_e = AlignLeft);
};

#endif
