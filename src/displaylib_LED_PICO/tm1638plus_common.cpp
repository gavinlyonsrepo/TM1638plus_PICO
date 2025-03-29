/*!
	@file     tm1638plus_common.cpp
	@author   Gavin Lyons
	@brief     cpp  file for common data and functions between model 1 and 2 classes
*/

#include "pico/stdlib.h"
#include "displaylib_LED_PICO/tm1638plus_common.hpp"

/*!
	@brief Constructor for class TM1638plus_common
	@param strobe  GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
*/
TM1638plus_common::TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data)
{
	_STROBE_IO = strobe;
	_DATA_IO = data;
	_CLOCK_IO = clock;
}

/*!
	@brief Begin method , sets pin modes and activate display.
*/
void TM1638plus_common::displayBegin()
{
	gpio_init(_STROBE_IO);
	gpio_init(_DATA_IO);
	gpio_init(_CLOCK_IO);
	gpio_set_dir(_STROBE_IO, GPIO_OUT);
	gpio_set_dir(_DATA_IO, GPIO_OUT);
	gpio_set_dir(_CLOCK_IO, GPIO_OUT);
	sendCommand(TM_ACTIVATE);
	brightness(TM_DEFAULT_BRIGHTNESS);
	reset();
}

/*!
	@brief Close method , clears display, deinits pin modes and deactivate display.
*/
void TM1638plus_common::displayClose()
{
	reset();
	busy_wait_ms(50);
	gpio_put(_STROBE_IO, false);
	gpio_put(_DATA_IO, false);
	gpio_put(_CLOCK_IO, false);
	busy_wait_ms(50);
	gpio_deinit(_STROBE_IO);
	gpio_deinit(_DATA_IO);
	gpio_deinit(_CLOCK_IO);
}

/*!
	@brief Send command to display
	@param value command byte to send
*/
void TM1638plus_common::sendCommand(uint8_t value)
{
	gpio_put(_STROBE_IO, false);
	sendData(value);
	gpio_put(_STROBE_IO, true);
}

/*!
	@brief Send Data to display
	@param data  Data byte to send
*/
void TM1638plus_common::sendData(uint8_t data)
{
	HighFreqshiftOut(_DATA_IO, _CLOCK_IO, data);
}

/*!
	@brief Reset / clear  the  display
	@note The display is cleared by writing zero to all data segment  addresses.
*/
void TM1638plus_common::reset()
{
	sendCommand(TM_WRITE_INC); // set auto increment mode
	gpio_put(_STROBE_IO, false);
	sendData(TM_SEG_ADR); // set starting address to
	for (uint8_t i = 0; i < 16; i++)
	{
		sendData(0x00);
	}
	gpio_put(_STROBE_IO, true);
}

/*!
	@brief  Sets the brightness level of segments in display on a scale of brightness
	@param brightness byte with value 0 to 7 The DEFAULT_BRIGHTNESS = 0x02
*/
void TM1638plus_common::brightness(uint8_t brightness)
{
	uint8_t value = 0;
	value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
	sendCommand(value);
}

/*!
	@brief    Shifts in a byte of data from the Tm1638 SPI-like bus
	@param dataPin Tm1638 Data GPIO
	@param clockPin Tm1638 Clock GPIO
	@return  Data byte
*/
uint8_t TM1638plus_common::HighFreqshiftin(uint8_t dataPin, uint8_t clockPin)
{
	uint8_t value = 0;
	uint8_t i = 0;

	for (i = 0; i < 8; ++i)
	{

		gpio_put(clockPin, true);
		busy_wait_us(_HFIN_DELAY);
		value |= gpio_get(dataPin) << i;
		gpio_put(clockPin, false);
		busy_wait_us(_HFIN_DELAY);
	}
	return value;
}

/*!
	@brief    Shifts out a byte of data on to the Tm1638 SPI-like bus
	@param dataPin Tm1638 Data GPIO
	@param clockPin Tm1638 Clock GPIO
	@param val The byte of data to shift out
*/
void TM1638plus_common::HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		gpio_put(dataPin, !!(val & (1 << i)));
		gpio_put(clockPin, true);
		busy_wait_us(_HFIN_DELAY);
		gpio_put(clockPin, false);
		busy_wait_us(_HFIN_DELAY);
	}
}
