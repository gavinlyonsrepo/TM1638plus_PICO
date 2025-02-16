/*!
	@file     tm1638plus_common.h
	@author   Gavin Lyons
	@brief    PICO library Tm1638plus_PICO, Header file for common data and functions between model classes.
	@note     Project URL https://github.com/gavinlyonsrepo/TM1638plus_PICO
*/

#ifndef TM1638PLUS_COMMON_H
#define TM1638PLUS_COMMON_H

extern const uint8_t *pFontSevenSegptr; /**<  Pointer to the seven segment font data table */

/*!
	@brief  The base Class , used to store common data & functions for all models types.
*/
class TM1638plus_common
{

public:
	// Constructor
	TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data);

	/*! Led color values  */
	enum TMLedColor_e : uint8_t
	{
		TM_OFF_LED = 0x00,	 /**< Switch off LED */
		TM_GREEN_LED = 0x01, /**< Turn LED green color (Model 3 only) */
		TM_RED_LED = 0x02	 /**< Turn LED red color (Model 3 only) */
	};

	/*! Alignment of text on display */
	enum AlignTextType_e : uint8_t
	{
		AlignTextRight = 1, /**< Align text to the right on display */
		AlignTextLeft = 2	/**< Align text to the left  on display */
	};

	void displayBegin();
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

	/*! ASCII table calculation constants */
	enum TMConstants_e : uint8_t
	{
		TM_ASCII_OFFSET = 32, /**< ASCII table offset to jump over first missing 32 chars */
		TM_HEX_OFFSET = 16,	  /**< ASCII table offset to reach the number position */
		TM_DOT_MASK_DEC = 128 /**< 0x80 Mask to switch on decimal point in seven-segment */
	};

	uint8_t HighFreqshiftin(uint8_t dataPin, uint8_t clockPin);
	void HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val);
	void sendCommand(uint8_t value);
	void sendData(uint8_t data);

private:
};

#endif
