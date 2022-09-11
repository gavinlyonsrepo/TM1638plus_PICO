/*
* Project Name: TM1638plus 
* File: TM1638plus_common
* Description: cpp  file for common data and functions between model 1 and 2 classes
* The common class is the master class.
* Arduino library TM1638plus
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/TM1638plus_PICO
*/

#include "pico/stdlib.h"
#include "../include/tm1638/tm1638plus_common.h"


TM1638plus_common::TM1638plus_common(uint8_t strobe, uint8_t clock, uint8_t data)
{
  _STROBE_IO = strobe;
  _DATA_IO = data;
  _CLOCK_IO = clock;
}

void TM1638plus_common::displayBegin() {
  gpio_init(_STROBE_IO);
  gpio_init(_DATA_IO);
  gpio_init(_CLOCK_IO);
  gpio_set_dir(_STROBE_IO, GPIO_OUT);
  gpio_set_dir(_DATA_IO, GPIO_OUT);
  gpio_set_dir(_CLOCK_IO , GPIO_OUT);
  sendCommand(TM_ACTIVATE);
  brightness(TM_DEFAULT_BRIGHTNESS);
  reset();
}


void TM1638plus_common::sendCommand(uint8_t value)
{
  gpio_put(_STROBE_IO, false);
  sendData(value);
  gpio_put(_STROBE_IO, true);
}

void TM1638plus_common::sendData(uint8_t data)
{
    HighFreqshiftOut(_DATA_IO, _CLOCK_IO, data);
}

void TM1638plus_common::reset() {
  sendCommand(TM_WRITE_INC); // set auto increment mode
  gpio_put(_STROBE_IO, false);
  sendData(TM_SEG_ADR);  // set starting address to
  for (uint8_t i = 0; i < 16; i++)
  {
    sendData(0x00);
  }
   gpio_put(_STROBE_IO, true);
}

void TM1638plus_common::brightness(uint8_t brightness)
{
    uint8_t  value = 0;
    value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
    sendCommand(value);
}

uint8_t  TM1638plus_common::HighFreqshiftin(uint8_t dataPin, uint8_t clockPin) 
{
    uint8_t value = 0;
    uint8_t i = 0;

    for(i = 0; i < 8; ++i) {
        
        gpio_put(clockPin, true);
        busy_wait_us(TM_HFIN_DELAY);
        value |= gpio_get(dataPin) << i;
        gpio_put(clockPin, false);
        busy_wait_us(TM_HFIN_DELAY);
    }
    return value;
}

void TM1638plus_common::HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        gpio_put(dataPin, !!(val & (1 << i)));  
        gpio_put(clockPin, true);
        busy_wait_us(TM_HFIN_DELAY);
        gpio_put(clockPin, false);
        busy_wait_us(TM_HFIN_DELAY);
    }
}
