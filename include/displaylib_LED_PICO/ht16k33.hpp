/*!
	@file   ht16k33.hpp
	@author Gavin Lyons
	@brief  Header file for for HT16k33 module. Model 6
	@todo
			-# hexadecimal string & number function,
			-# keyscan,
			-# leading zeros option to float and string function.
*/

#ifndef HT16k33_COMMON_H
#define HT16k33_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <algorithm> // Required for std::fill
#include <cstring>
#include <cmath> // pow log10

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "common_data.hpp"
#include "seven_segment_font_data.hpp"
#include "nine_segment_font_data.hpp"
#include "fourteen_segment_font_data.hpp"
#include "sixteen_segment_font_data.hpp"

/*! @brief class to control Ht16K33 , supports 7 9 14 and 16 segment displays */
class HT16K33plus_model6 : public SevenSegmentFont, public  NineSegmentFont, 
					public FourteenSegmentFont, public SixteenSegmentFont , public CommonData
{
	public:
		// enums:
		/*!
		 * Display blinking frequency XXXX-XB1B0X
		 * settings Display register D10 D9 -> (B1, B0)
		*/
		enum BlinkFreq_e : uint8_t
		{
			BLINKOFF  =    0x81, /**< Blinking off*/
			BLINKON2HZ =   0x83, /**< 2 Hertz*/
			BLINKON1HZ =   0x85, /**< 1 Hertz*/
			BLINKON0_5HZ = 0x87  /**< 0.5 Hertz*/
		};
		/*! The Display Type */
		enum DisplayType_e : uint8_t
		{
			SegType7  = 7,  /**< 7 segment display */
			SegType9  = 9,  /**< 9 segment display */
			SegType14 = 14, /**< 14 segment display */
			SegType16 = 16  /**< 16 segment display */
		};
		// constructor  
		 HT16K33plus_model6(uint8_t address, i2c_inst_t* i2c_type, uint8_t SDApin, uint8_t SCLKpin, uint16_t CLKspeed);
		// methods I2C related
		void Display_I2C_ON(void);
		void Display_I2C_OFF(void);
		int DisplayCheckConnection(void);
		int DisplayI2CErrorGet(void) const;
		uint16_t DisplayI2CErrorTimeoutGet(void) const;
		void DisplayI2CErrorTimeoutSet(uint16_t);
		uint8_t DisplayI2CErrorRetryNumGet(void) const;
		void DisplayI2CErrorRetryNumSet(uint8_t);
		uint32_t DisplayI2CTimeoutCommsGet() const;
		void DisplayI2CTimeoutCommsSet(uint32_t timeout);

		// Device related
		void DisplayInit(uint8_t brightLevel, BlinkFreq_e blink,
			uint8_t numOfDigits, DisplayType_e dtype);
		void DisplayOn(void);
		void DisplayOff(void);
		void DisplaySleep(void);
		void DisplayNormal(void);
		void DisplayResetDefault(void);
		void setBrightness(uint8_t value);
		uint8_t getBrightness() const;
		void  setBlink(BlinkFreq_e  value);
		BlinkFreq_e  getBlink() const;
		DisplayType_e getDisplayType() const;

		// Display data related
		void ClearDigits(void);
		//raw data segments
		void displayRawData(uint8_t digitPos, uint16_t value);
		// Display data
		int displayChar(uint8_t digitPos, char c, DecimalPoint_e dp);
		int displayText(const char *text, TextAlignment_e TextAlignment);
		int displayText(const char *text);
		int displayHexChar(uint8_t digitPos, char hex);
		int displayIntNum(int32_t number, TextAlignment_e TextAlignment);
		int displayFloatNum(float number, TextAlignment_e TextAlignment, uint8_t fractionDigits);
	protected:

	private:
		int displayMultiSegNum(uint8_t digitPos, char c, DecimalPoint_e dp);

		// methods I2C related
		void SendCmd(uint8_t cmd);
		void SendData(const unsigned char* data, size_t length);

		// Members I2C related
		i2c_inst_t* _i2cInterface = i2c0;   /**< I2C instance, 0 or 1 */
		uint8_t      _address      = 0x70;  /**< I2C address */
		uint8_t     _SDataPin     = 16;     /**< I2CX SDA GPIO pin  */
		uint8_t     _SClkPin      = 17;     /**< I2CX SCL GPIO pin  */
		uint16_t    _CLKSpeed     = 100;    /**< I2C bus speed in khz typically 100-400 */
		uint16_t  _I2C_ErrorDelay = 100;    /**< I2C delay(in between retry attempts) in event of error in mS*/
		uint8_t  _I2C_ErrorRetryNum = 3;    /**< In event of I2C error number of retry attempts*/
		int      _I2C_ErrorFlag     = 0;    /**< In event of I2C error, holds code*/
		uint32_t _I2C_TimeoutComms = 50000; /**< Timeout for I2C comms, uS,*/

		// Display settings
		BlinkFreq_e _blinkSetting = BLINKOFF;   /**< Blink setting, 4 settings see enum */
		DisplayType_e  _displayType = SegType7; /**< Enum to hold chosen display type */
		uint8_t _brightness = 7;                /**< Brightness setting 0-15 */
		uint8_t _numOfDigits = 4;               /**< Number of digits in display max 8 */

		//  Register Command List
		static constexpr uint8_t HT16K33_DDAPTR =     0x00; /**< Display data address pointer */
		static constexpr uint8_t HT16K33_NORMAL =     0x21; /**< System setup register turn on System oscillator, normal operation mode */
		static constexpr uint8_t HT16K33_STANDBY =    0x20; /**< Turn off System oscillator, standby mode */
		static constexpr uint8_t HT16K33_DISPLAYON =  0x81; /**< Display set register  Display on */
		static constexpr uint8_t HT16K33_DISPLAYOFF = 0x80; /**< Display set register  Display off */
		static constexpr uint8_t HT16K33_BRIGHTNESS = 0xE0; /**< Dimming set register 0-15 XXXX-BBBB*/

};

#endif