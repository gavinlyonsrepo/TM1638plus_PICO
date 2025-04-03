/*!
	@file     tm1638plus_common.hpp
	@author   Gavin Lyons
	@brief    tm1638  PICO driver. Header file for common data and functions between model classes.
*/

#ifndef TM1638PLUS_COMMON_H
#define TM1638PLUS_COMMON_H

#include "common_data.hpp"
#include "seven_segment_font_data.hpp"
#include <cstdio>

/*!
	@brief  The base Class , used to store common data & functions for all models types.
*/
class TM1638plus_common : public SevenSegmentFont , public CommonData
{

public:
	// Constructor
	TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data);

	void displayBegin(void);
	void displayClose(void);
	void reset(void);
	void brightness(uint8_t brightness);

protected:
	uint8_t _STROBE_IO; /**<  GPIO connected to STB on Tm1638  */
	uint8_t _DATA_IO;	/**<  GPIO connected to DIO on Tm1638  */
	uint8_t _CLOCK_IO;	/**<  GPIO connected to CLk on Tm1638  */

	uint8_t _HFIN_DELAY = 1;  /**<  uS Delay used by shiftIn function for High-freq MCU  */
	uint8_t _HFOUT_DELAY = 1; /**<  uS Delay used by shiftOut function for High-freq MCU */

	// Commands list and defaults
	static constexpr uint8_t TM_ACTIVATE = 0x8F;		   /**< Start up device */
	static constexpr uint8_t TM_BUTTONS_MODE = 0x42;	   /**< Buttons mode */
	static constexpr uint8_t TM_WRITE_LOC = 0x44;		   /**< Write to a memory location */
	static constexpr uint8_t TM_WRITE_INC = 0x40;		   /**< Incremental write */
	static constexpr uint8_t TM_SEG_ADR = 0xC0;			   /**< Leftmost segment Address C0 C2 C4 C6 C8 CA CC CE */
	static constexpr uint8_t TM_LEDS_ADR = 0xC1;		   /**< Leftmost LED address C1 C3 C5 C7 C9 CB CD CF */
	static constexpr uint8_t TM_BRIGHT_ADR = 0x88;		   /**< Brightness address */
	static constexpr uint8_t TM_BRIGHT_MASK = 0x07;		   /**< Brightness mask */
	static constexpr uint8_t TM_DEFAULT_BRIGHTNESS = 0x02; /**< Brightness can be 0x00 to 0x07, 0x00 is least bright */
	static constexpr uint8_t TM_DISPLAY_SIZE = 8;		   /**< Size of display in digits */

	uint8_t HighFreqshiftin(uint8_t dataPin, uint8_t clockPin);
	void HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val);
	void sendCommand(uint8_t value);
	void sendData(uint8_t data);

private:
};

#endif
