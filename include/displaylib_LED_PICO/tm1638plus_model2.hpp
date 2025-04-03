/*!
	@file   tm1638plus_model2.hpp
	@author Gavin Lyons
	@brief  PICO library, Header file for TM1638 module(16 KEY 16 pushbuttons. Model 2.
*/

#ifndef TM1638PLUS_MODEL2_H
#define TM1638PLUS_MODEL2_H

#include "pico/stdlib.h"
#include <cstdio>
#include <cstring>
#include "tm1638plus_common.hpp"

/*!
	@brief Class for Model 2
*/
class TM1638plus_model2 : public TM1638plus_common
{

public:
	// Constructor Init the module
	TM1638plus_model2(uint8_t strobe, uint8_t clock, uint8_t data, bool swap_nibbles);

	// Methods
	uint8_t ReadKey16(void);
	uint16_t ReadKey16Two(void);

	void DisplaySegments(uint8_t segment, uint8_t segmentValue);
	void DisplayHexNum(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, TextAlignment_e = AlignLeft);
	void DisplayDecNum(unsigned long number, uint8_t dots, TextAlignment_e = AlignLeft);
	int  DisplayStr(const char *string, const uint16_t dots = 0);
	void ASCIItoSegment(const uint8_t values[]);
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, TextAlignment_e = AlignLeft);

private:
	bool _SWAP_NIBBLES = false; /**< If true the nibbles in display byte will be switched AAAABBBB BBBBAAAA */
};

#endif
