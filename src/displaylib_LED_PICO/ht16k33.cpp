/*!
	@file   ht16k33.cpp
	@author Gavin Lyons
	@brief  Source file for for HT16k33 module. Model 6
	@todo
			-# hexadecimal string & number function,
			-# keyscan,
			-# leading zeros option to float and string function.
*/

#include "../../include/displaylib_LED_PICO/ht16k33.hpp"


/*!
	@brief Constructor for class HT16K33plus_model6
	@param address  I2C address
	@param i2c_type IC2 interface , ic20 or i2c1 
	@param SDApin   Data pin I2C
	@param SCLKpin  Clock pin I2C
	@param CLKspeed I2C Clock speed in Khz 0-400Khz
*/
HT16K33plus_model6::HT16K33plus_model6(uint8_t address, i2c_inst_t* i2c_type, uint8_t SDApin, uint8_t SCLKpin, uint16_t CLKspeed) 
{
	_address = address;
	_i2cInterface = i2c_type;
	_SDataPin = SDApin;
	_SClkPin =  SCLKpin;
	_CLKSpeed = CLKspeed;  
}

/*!
	@brief  Send data buffer to  via I2C
	@param data The data buffer to send
	@param length length of data to send
	@note if debug flag is true, will output data on I2C failures.
*/
void HT16K33plus_model6::SendData(const unsigned char* data, size_t length) {
	
	uint8_t AttemptCount = _I2C_ErrorRetryNum;
	int ErrorCode = i2c_write_timeout_us(_i2cInterface, _address, data, length, false, _I2C_TimeoutComms);
	// Error handling retransmit
	while (ErrorCode < 1) {
		if (CommonData::displaylib_LED_debug) {
			printf("Error: SendData I2C: %i \n", ErrorCode);
			printf("Attempt Count: %u \n", AttemptCount);
		}
		busy_wait_ms(_I2C_ErrorDelay);
		ErrorCode = i2c_write_timeout_us(_i2cInterface, _address, data, length, false, _I2C_TimeoutComms); // retransmit
		_I2C_ErrorFlag = ErrorCode;
		AttemptCount--;
		if (AttemptCount == 0) break;
	}
	_I2C_ErrorFlag = ErrorCode;
}


/*!
	@brief  Send command byte to display
	@param cmd command byte
	@note if debug flag == true  ,will output data on I2C failures.
*/
void HT16K33plus_model6::SendCmd(uint8_t cmd) {

	uint8_t cmdBufferI2C[1];
	cmdBufferI2C[0] = cmd;
	uint8_t AttemptCount = _I2C_ErrorRetryNum;
	int ErrorCode = i2c_write_timeout_us(_i2cInterface, _address, cmdBufferI2C, sizeof(cmdBufferI2C), false, _I2C_TimeoutComms);
	// Error handling retransmit
	while(ErrorCode < 1)
	{
		if (CommonData::displaylib_LED_debug)
		{
			printf("Error:  SendCmd I2C: %i\n", ErrorCode);
			printf("Attempt Count: %u \n", AttemptCount );
		}
		busy_wait_ms(_I2C_ErrorDelay );
		ErrorCode = i2c_write_timeout_us(_i2cInterface, _address, cmdBufferI2C, sizeof(cmdBufferI2C), false, _I2C_TimeoutComms); // retransmit
		_I2C_ErrorFlag = ErrorCode;
		AttemptCount--;
		if (AttemptCount == 0) break;
	}
	_I2C_ErrorFlag = ErrorCode;
}

/*!
	@brief Initialise I2C operations 
*/
void HT16K33plus_model6::Display_I2C_ON(void)
{
	gpio_set_function(_SDataPin, GPIO_FUNC_I2C);
    gpio_set_function(_SClkPin, GPIO_FUNC_I2C);
	gpio_pull_up(_SDataPin);
    gpio_pull_up(_SClkPin);
	i2c_init(_i2cInterface, _CLKSpeed * 1000);
}

/*!
	@brief End I2C operations
*/
void HT16K33plus_model6::Display_I2C_OFF(void)
{
	gpio_set_function(_SDataPin, GPIO_FUNC_NULL);
	gpio_set_function(_SClkPin, GPIO_FUNC_NULL);
	i2c_deinit(_i2cInterface);
}

/*!
	@brief get I2C error Flag
	@details Number of bytes written, or PICO_ERROR_GENERIC if address not acknowledged, 
		no device present, or PICO_ERROR_TIMEOUT if a timeout occurred.
	@return I2C error flag
*/
int HT16K33plus_model6::DisplayI2CErrorGet(void) const { return _I2C_ErrorFlag;}

/*!
	@brief Sets the I2C timeout, in the event of an I2C write error
	@param newTimeout I2C timeout delay in mS
	@details Delay between retry attempts in event of an error , mS
*/
void HT16K33plus_model6::DisplayI2CErrorTimeoutSet(uint16_t newTimeout)
{
	_I2C_ErrorDelay = newTimeout;
}

/*!
	@brief Gets the I2C timeout, used in the event of an I2C write error
	@details Delay between retry attempts in event of an error , mS
	@return  I2C timeout delay in mS, _I2C_ErrorDelay
*/
uint16_t HT16K33plus_model6::DisplayI2CErrorTimeoutGet(void) const{return _I2C_ErrorDelay;}

/*!
	@brief Gets the I2C error retry attempts, used in the event of an I2C write error
	@details Number of times to retry in event of an error
	@return   _I2C_ErrorRetryNum
*/
uint8_t HT16K33plus_model6::DisplayI2CErrorRetryNumGet(void) const {return _I2C_ErrorRetryNum;}

/*!
	@brief Sets the I2C error retry attempts used in the event of an I2C write error
	@details Number of times to retry in event of an error
	@param AttemptCount I2C retry attempts
*/
void HT16K33plus_model6::DisplayI2CErrorRetryNumSet(uint8_t AttemptCount)
{
	_I2C_ErrorRetryNum = AttemptCount;
}

/*!
	@brief checks if Display on I2C bus
	@return Error codes Number of bytes written, or PICO_ERROR_GENERIC if address not acknowledged, 
		no device present, or PICO_ERROR_TIMEOUT if a timeout occurred.
		Prints error code text to console
*/
int HT16K33plus_model6::DisplayCheckConnection(void)
{
	uint8_t rxdatabuf[1]; //buffer to hold return byte
	int I2CReadStatus = 0;
	I2CReadStatus = i2c_read_timeout_us(_i2cInterface, _address, rxdatabuf, 
			sizeof(rxdatabuf), false, _I2C_TimeoutComms);
	if (I2CReadStatus < 1 )
	{
		printf("Error: DisplayCheckConnection :Cannot read device (%i)\n",I2CReadStatus);
	}
	_I2C_ErrorFlag = I2CReadStatus;
	return _I2C_ErrorFlag;
}


/*!
	@brief Initializes the HT16K33 display with specified settings.
	@param brightLevel Brightness level (0-15). If greater than 15, it defaults to 15.
	@param blinklevel Blink frequency setting (enumeration BlinkFreq_e 4 settings).
	@param numOfDigits Number of digits to be displayed.
	@param displayType Type of display configuration (enumeration DisplayType_e 4 settings).
*/
void HT16K33plus_model6::DisplayInit(uint8_t brightLevel, BlinkFreq_e  blinklevel, uint8_t numOfDigits, DisplayType_e displayType)
{
	SendCmd(HT16K33_NORMAL); //normal operation mode
	SendCmd(HT16K33_BRIGHTNESS + brightLevel);//brightness level
	SendCmd(HT16K33_DISPLAYON | blinklevel); //display ON
	if (brightLevel > 0x0F) {
		printf("Warning : DisplayInit : Brightness value must be lower than 17 setting to 16\n");
		brightLevel = 0x0F;
	}
	_blinkSetting = blinklevel;
	_brightness = brightLevel;
	_numOfDigits = numOfDigits;
	_displayType = displayType;
}

/*!
	@brief Turns on the display with the previously set blink setting.
	@details Sends the command to enable the display, using the stored blink setting.
*/
void HT16K33plus_model6::DisplayOn(void){
	SendCmd(HT16K33_DISPLAYON | _blinkSetting);
}

/*!
	@brief Turns off the display.
*/
void HT16K33plus_model6::DisplayOff(void){
	SendCmd(HT16K33_DISPLAYOFF);
}

/*!
	@brief Puts the display into standby mode. Turn off System oscillator
*/
void HT16K33plus_model6::DisplaySleep(void){
	SendCmd(HT16K33_STANDBY);
}

/*!
	@brief Restores the display to normal operation mode. Turn on System oscillator
*/
void HT16K33plus_model6::DisplayNormal(void){
	SendCmd(HT16K33_NORMAL);
}

/*!
	@brief Resets the display to default settings.
	@details This function restores normal operation mode, sets brightness to a default level of 7,
	         and turns the display on with the stored blink setting.
*/
void HT16K33plus_model6::DisplayResetDefault(void){
	DisplayNormal();
	setBrightness(7);
	DisplayOn();
}


/*!
	@brief Sets the display blink frequency.
	@param blinklevel Blink frequency setting (enumeration BlinkFreq_e 4 settings).
*/
void HT16K33plus_model6::setBlink( BlinkFreq_e  blinklevel){
	_blinkSetting = blinklevel;
	SendCmd(HT16K33_DISPLAYON | _blinkSetting );
}

/*!
	@brief Gets the current blink frequency setting.
	@returns The current blink frequency (enumeration BlinkFreq_e).
*/
HT16K33plus_model6::BlinkFreq_e HT16K33plus_model6::getBlink() const{
	return  _blinkSetting;
}

/*!
	@brief Gets the display type.
	@returns The current display type setting (enumeration DisplayType_e).
*/
HT16K33plus_model6::DisplayType_e HT16K33plus_model6::getDisplayType() const{
	return  _displayType;
}

/*!
	@brief Sets the display brightness level.
	@param value Brightness level (0-15). If greater than 15, it defaults to 15.
*/
void HT16K33plus_model6::setBrightness(uint8_t value)
{
	if (value == _brightness) 
		return;
	_brightness = value;
	if (_brightness > 0x0F) {
		printf("Warning : setBrightness : Brightness value must be lower than 17 setting to 16\n");
		_brightness = 0x0F;
	}
	SendCmd(HT16K33_BRIGHTNESS + _brightness);
}

/*!
	@brief Gets the current brightness level.
	@returns The current brightness level (0-15).
*/
uint8_t HT16K33plus_model6::getBrightness() const{
	return _brightness;
}

/*!
	@brief Displays a single character at the specified digit position.
	@param digitPosition The position of the digit on the display (0-based index 0 = LHS).
	@param character The ASCII character to display.
	@param decimalOnPoint Specifies whether the decimal point should be enabled (enumeration DecimalPoint_e).
	@returns Return code indicating success or an error (enumeration int).
	@details If the character is out of the supported ASCII font range, an error is logged and a corresponding error code is returned.
	         If the display type is not 7-segment, the function delegates to displayMultiSegNum().
	         Otherwise, the function retrieves the 7-segment font data, applies the decimal point if needed, 
	         and sends the data to the display.
*/
int HT16K33plus_model6::displayChar(uint8_t digitPosition, char character, DecimalPoint_e decimalOnPoint)
{
	if (character < (_ASCII_FONT_OFFSET) || character >=_ASCII_FONT_END )
	{// check ASCII font bounds
		printf("Error : displayChar: ASCII character is outside font range %c, \n", character);
		return -5;
	}
	if (_displayType != SegType7){
			return displayMultiSegNum(digitPosition, character, decimalOnPoint);
	} else {
		uint8_t characterConverted;
		const uint8_t* font = SevenSegmentFont::pFontSevenSegptr();
		characterConverted = font[character- _ASCII_FONT_OFFSET];
		if (decimalOnPoint ==  DecPointOn) characterConverted |= DEC_POINT_7_MASK;
		uint8_t txDataBuffer[2];
		size_t length = sizeof(txDataBuffer);
		txDataBuffer[0] = digitPosition*2;
		txDataBuffer[1] = characterConverted;
		SendData(txDataBuffer, length);
	}
	return 0;
}

/*!
	@brief Displays a character on multi-segment displays (9, 14, or 16 segments).
	@param digitPosition The position of the digit on the display (0-based index, 0=LHS).
	@param character The ASCII character to display.
	@param decimalOnPoint Specifies whether the decimal point should be enabled (enumeration DecimalPoint_e).
	@returns Return code indicating success or an error (enumeration int).
	@details Converts the ASCII character into the appropriate segment mapping for the display type (9-segment, 14-segment, or 16-segment).
	         The decimal point is applied if applicable.
	         The processed data is then sent to the display.
*/
int HT16K33plus_model6::displayMultiSegNum(uint8_t digitPosition, char character, DecimalPoint_e decimalOnPoint)
{
	uint16_t characterConverted = 0;
	switch (_displayType)
	{
		case SegType7:
			return -9; // this will never occur in normal user operation
		break; 
		case SegType9:{
			const uint16_t* fontNine = NineSegmentFont::pFontNineSegptr();
			characterConverted = fontNine[character- _ASCII_FONT_OFFSET ];
			if (decimalOnPoint ==  DecPointOn) characterConverted |= DEC_POINT_9_MASK;
			}
		break;
		case SegType14:{
			const uint16_t* fontFourteen = FourteenSegmentFont::pFontFourteenSegptr();
			characterConverted = fontFourteen[character- _ASCII_FONT_OFFSET ];
			if (decimalOnPoint ==  DecPointOn) characterConverted |= DEC_POINT_14_MASK;
			}
		break;
		case SegType16:{
			const uint16_t* fontSixteen = SixteenSegmentFont::pFontSixteenSegptr();
			characterConverted = fontSixteen[character- _ASCII_FONT_OFFSET ];
			(void)decimalOnPoint;
			}
		break;
	}
	uint8_t txDataBuffer[3];
	size_t bufferLength= sizeof(txDataBuffer);
	txDataBuffer[0] = digitPosition * 2;
	txDataBuffer[1] = characterConverted & 0x00FF;
	txDataBuffer[2] = (characterConverted & 0xFF00) >> 8;
	SendData(txDataBuffer, bufferLength);
	return 0;
}

/*!
	@brief Sends raw segment data to a specific digit position.
	@param digitPosition The position of the digit on the display (0-based index).
	@param rawData The raw segment data to be displayed (bit-mapped for the display type).
	@details This function allows direct control of the display segments by sending raw data.
	         The rawData value is split into two bytes and transmitted to the display.
*/
void HT16K33plus_model6::displayRawData(uint8_t digitPosition, uint16_t rawData)
{
	uint8_t txDataBuffer[3];
	size_t bufferLength = sizeof(txDataBuffer); 
	txDataBuffer[0] = digitPosition * 2;
	txDataBuffer[1] = rawData & 0x00FF;
	txDataBuffer[2] = (rawData & 0xFF00) >> 8;
	SendData(txDataBuffer, bufferLength);
}

/*!
	@brief Clears all digits on the display.
	@details Iterates through all digit positions and replaces each character with a blank space.
	         The decimal points are also turned off.
*/
void HT16K33plus_model6::ClearDigits(void)
{
	for(uint8_t i=0; i<=_numOfDigits; i++) 
	{
		displayChar(i, ' ', DecPointOff);
	}
}

/*!
	@brief Displays a text string on display
	@param text pointer to character array containg text string
	@param TextAlignment  left or right alignment
	@note This method is overloaded, see also DisplayText(char *)
		leading zeros is not currently an option as a workaround
		user can add them to string before hand.
	@return WIll return error for null pointer string or leading zeros option requested
*/
int HT16K33plus_model6::displayText(const char *text, TextAlignment_e TextAlignment) {
	if (text == nullptr) {
		printf("Error: displayText: String is a null pointer.\n");
		return -2;
	}
	uint8_t charCount = strlen(text); 
	// Adjust character count for non-16 segment types by subtracting non-consecutive dots
	if (_displayType != SegType16)
	{
		for (uint8_t i = 1; text[i]; i++) {
			if (text[i] == '.' && text[i - 1] != '.') {
				charCount--; 
			}
		}
	}
	if (charCount > _numOfDigits) charCount = _numOfDigits;
	// Compute starting position based on alignment
	uint8_t pos = 0;
	switch (TextAlignment)
	{
		case AlignLeft: pos = 0; break;
		case AlignRight: pos = _numOfDigits - charCount; break;
		case AlignRightZeros:
			printf("Error: displayText 2: Leading zeros not an option in this function\n");
			return -9; 
		break;
	}
	// Display characters with correct alignment
	uint8_t displayPos = pos;
	while (*text && displayPos < _numOfDigits)
	{
		char character = *text++;
		if (_displayType != SegType16 && *text == '.' && character != '.') {
			displayChar(displayPos++, character, DecPointOn);
			text++;  // Skip the decimal point
		} else {
			displayChar(displayPos++, character, DecPointOff);
		}
	}
	return 0;
}


/*!
	@brief Display a text string  on display
	@param text pointer to a character array
	@return error code  if string is nullptr or too long 
	@note 
		Dots are removed from string and dot on preceding digit switched on
		"abc.def" will be shown as "abcdef" with c decimal point turned on,
		Unless the Display is sixteen segment.
*/
int HT16K33plus_model6::displayText(const char *text) {
	// Check for null pointer
	if (text == nullptr) 
	{
		fprintf(stderr ,"Error: displayText1: String is a null pointer.\n");
		return -2;
	}
	char character, pos;
	pos = 0;
	while ((character = (*text++)) && pos < _numOfDigits)
		{
			if (_displayType != SegType16 && *text == '.' && character != '.') {
				displayChar(pos++ ,character, DecPointOn);
				text++;
			}else{
				displayChar(pos++ ,character, DecPointOff);
			}
		}
	return 0;
}

/*!
	@brief  Send Hexadecimal value to seven segment
	@param position The position on display 
	@param hex  hexadecimal  value (DEC) 0-15  (0x00 - 0x0F)
	@return Will return an error if not hexadecimal.
*/
int  HT16K33plus_model6::displayHexChar(uint8_t position, char hex) 
{
		if (hex <= '9')
		{
			displayChar(position, hex , DecPointOff);
		}else
		{
			char offset = 0;
			switch(hex) 
			{
			 case 'A': offset = 'A'; break;
			 case 'B': offset = 'b'; break;
			 case 'C': offset = 'C'; break;
			 case 'D': offset = 'd'; break;
			 case 'E': offset = 'E'; break;
			 case 'F': offset = 'F'; break;
			 default: 
				fprintf(stderr,"Error: displayHexChar:  This is not a hexadecimal character\n");
				return -5;
				break;
			}
			displayChar(position , offset , DecPointOff);
		}
	return 0;
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32 
	@param TextAlignment enum text alignment, left or right alignment or leading zeros
	@return will return error user tries to display  if too much data
*/
int HT16K33plus_model6::displayIntNum(int32_t number, TextAlignment_e TextAlignment)
{
	// Ensure the number can fit within the display
	if (number >= pow(10, _numOfDigits) || number < -(pow(10, (_numOfDigits-1)))) {
		printf("Error: displayIntNum: Number too many digits for display\n");
		return -9; 
	}
	char values[_numOfDigits + 1] = {0};  
	char TextDisplay[12]; 
	// Generate the format string dynamically
	switch (TextAlignment) 
	{
		case AlignRight:
			snprintf(TextDisplay, sizeof(TextDisplay), "%%%dd", (int)_numOfDigits);
			break;
		case AlignLeft:
			snprintf(TextDisplay, sizeof(TextDisplay), "%%-d"); 
			break;
		case AlignRightZeros:
			snprintf(TextDisplay, sizeof(TextDisplay), "%%0%dd", (int)_numOfDigits);
			break;
	}
	// Print formatted number into values buffer
	snprintf(values, sizeof(values), TextDisplay, number);
	displayText(values);
	return 0;
}

/*!
	@brief Displays a floating-point number on the display.
	@param number The floating-point number to be displayed.
	@param TextAlignment Text alignment option (enumeration TextAlignment_e 2 options).
	@param fractionDigits Number of fractional digits to display.
	@returns Return code indicating success or an error (enumeration int).
	@details Converts the floating-point number into a formatted string and displays it on the device.
	         If the total number of required digits (integer + fractional + sign) exceeds the available display space,
	         an error is returned. Leading zero alignment is not supported.
*/
int HT16K33plus_model6::displayFloatNum(float number, TextAlignment_e TextAlignment, uint8_t fractionDigits)
{
	if (TextAlignment == AlignRightZeros){
			printf("Error: displayFloatNum 2: Leading zeros not an option\n");
			return -9; 
	}
	// Calculate integer and fractional part lengths
	int intPart = (int)number;
	int intLen = (intPart == 0) ? 1 : (int)log10(abs(intPart)) + 1;  // Length of integer part
	int totalLen = intLen + fractionDigits;  // Total length includes integer and fractional digits
	if (number < 0) {
		totalLen++;  // Add space for the negative sign if needed
	}
	// Check if the number fits within the available display space
	if (totalLen > _numOfDigits) {
		printf("Error: displayFloatNum: Number too large for display (Total digits: %d, Max allowed: %d)\n", totalLen, _numOfDigits);
		return -9;
	}
	// Allocate buffer to store the formatted string
	char workStr[15];
	snprintf(workStr, sizeof(workStr), "%.*f", fractionDigits, number);
	displayText(workStr, TextAlignment);
	return 0;
}

/*!
	@brief  Gets the timeout value in uS of the timeout delay used in I2C communications
	@details The time that the function will wait for the entire transaction to complete
	@return timeout delay uS, default 50000
*/
uint32_t HT16K33plus_model6::DisplayI2CTimeoutCommsGet() const {
	return _I2C_TimeoutComms;
}

/*!
	@brief  Sets the timeout value in uS of the timeout delay used in I2C communications
	@details The time that the function will wait for the entire transaction to complete
	@param timeout Timeout delay uS, default 50000
*/
void HT16K33plus_model6::DisplayI2CTimeoutCommsSet(uint32_t timeout) {
	_I2C_TimeoutComms = timeout;
}
