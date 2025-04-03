/*!
	@file main.cpp
	@brief Example file for HT16k33 display 14 segment
	@details test for 14 segment display, the display used is LDD-F5406RI
		Common Cathode, 2 Digit , LT0565GWk, wiring connections in readme.
	@test
		-# Test 1 Brightness
		-# Test 2 Blink settings 
		-# Test 3 Display raw data, no ASCII font 
		-# Test 4 Standby mode
		-# Test 5 On/Off modes
		-# Test 6 Display the available members
		-# Test 7 Display characters
		-# Test 8 Display strings
		-# Test 9 Display hexadecimal characters
		-# Test 11 Display positive integers 
		-# Test 11 Display negative integers 
		-# Test 13 Display floating point numbers 
*/

// Included library
#include "displaylib_LED_PICO/ht16k33.hpp"

/// @cond

// Test timing
#define DISPLAY_DELAY_1 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY_5 5000


// Display
const uint8_t  numberofDigits = 2;
const uint8_t  brightness = 8;
const uint8_t  I2C_Address = 0x70;
const uint8_t  SCLK_GPIO  = 17;
const uint8_t  SDATA_GPIO = 16;
const uint16_t clockSpeed = 100;
HT16K33plus_model6 myHT(I2C_Address, i2c0, SDATA_GPIO, SCLK_GPIO, clockSpeed);

// Function Prototypes
bool setup(void);
void TestBright(void);
void TestBlink(void);
void TestRaw(void);
void TestStandby(void);
void TestOnOff(void);
void TestPrintFlags(void);
void TestChar(void);
void TestString(void);
void TestHex(void);
void TestIntPos(void);
void TestIntNeg(void);
void TestFloat(void);
void endTest(void);

// Main Loop
int main()
{
	if (!setup()) return -1;
	TestPrintFlags();
	TestRaw();
	TestBright();
	TestBlink();
	TestStandby();
	TestOnOff();
	TestChar();
	TestString();
	TestHex();
	TestIntPos();
	TestIntNeg();
	TestFloat();
	endTest();
	return 0;
} // END of main

// Functions

bool setup(void) {
	// Init USB output 38400 baud (optional, test messages and any errors)
	stdio_init_all();
	busy_wait_ms(DISPLAY_DELAY_1);
	printf(  "Test Begin\n");
	myHT.Display_I2C_ON();
	if (myHT.DisplayCheckConnection() < 0)// check on bus  ( Note optional)
	{
		printf( "Error 1202: Display not on bus?\n");
		return false;
	}else {
		printf( "CheckConnection passed: Display detected on the I2C bus\n");
	}
	busy_wait_ms(50);
	myHT.DisplayInit(brightness, myHT.BLINKOFF, numberofDigits, myHT.SegType14);
	busy_wait_ms(50);
	myHT.ClearDigits(); //Clear display
	return true;
}

void TestPrintFlags(void)
{
	// Print out flag status
	printf("Flags Status\n");
	printf("Brightness               : %u\n", myHT.getBrightness());
	printf("Blink intensity setting  : %u\n", myHT.getBlink());
	printf("Display Type             : %u\n", myHT.getDisplayType());
	printf("I2C Debug Error          : %i\n", myHT.DisplayI2CErrorGet());
	printf("I2C Error Timeout mS     : %u\n", myHT.DisplayI2CErrorTimeoutGet());
	printf("I2C Error retry attempts : %u\n", myHT.DisplayI2CErrorRetryNumGet());
	printf("I2C Comms Timeout uS     : %lu\n", myHT.DisplayI2CTimeoutCommsGet());
	printf("Library Version Number   : %s\n", myHT.displaylib_LED_VersionNum.c_str());
	printf("Debug flag enabled       : %s\n", myHT.displaylib_LED_debug ? "true" : "false");
}

void TestRaw(void)
{
	printf(  "Test Raw data on display\n");

	// Loop through each digit and segment to display each segment sequentially
	for (uint8_t digit : {0, 1}) {
		for (uint16_t segment = 0; segment <= 14; ++segment) {
			myHT.displayRawData(digit, 1 << segment);  // Set the specific segment for the digit
			busy_wait_ms(250);  // Wait for 1 second before moving to the next segment
		}
		myHT.displayRawData(digit, 0);  // Clear the digit after completing the segment loop
	}

	myHT.ClearDigits();
	busy_wait_ms(DISPLAY_DELAY_1);

	// More raw data will display  00.
	myHT.displayRawData(0, 0x0C3F); // raw data for 0
	myHT.displayRawData(1, 0x0C3F| 0x4000);  // raw data for 0 or'd with decimal point position
	busy_wait_ms(DISPLAY_DELAY_5);
}

void TestBright(void)
{
	printf(  "Test brightness\n");
	myHT.setBrightness(0x01); // min
	busy_wait_ms(DISPLAY_DELAY_5);
	myHT.setBrightness(0x0F); // max
	busy_wait_ms(DISPLAY_DELAY_5);
	myHT.setBrightness(brightness); //back to default
	busy_wait_ms(DISPLAY_DELAY_1);
}

void TestBlink(void)
{
	printf(  "Test blink mode\n");
	myHT.setBlink(myHT.BLINKON2HZ); // 2 Hertz , 4 options
	busy_wait_ms(DISPLAY_DELAY_5);
	myHT.setBlink(myHT.BLINKOFF);
	busy_wait_ms(DISPLAY_DELAY_1);//back to default
}

void TestStandby(void)
{
	printf(  "Test standby mode\n");
	myHT.DisplaySleep();
	busy_wait_ms(DISPLAY_DELAY_5);
	myHT.DisplayNormal();
	busy_wait_ms(DISPLAY_DELAY_2);
}

void TestOnOff(void)
{
	printf(  "Test on/off\n");
	myHT.DisplayOff();
	busy_wait_ms(DISPLAY_DELAY_5);
	myHT.DisplayOn();
	busy_wait_ms(DISPLAY_DELAY_2);
}

void TestChar(void)
{
	printf(  "Test single character 12\n");
	myHT.ClearDigits(); //Clear display
	busy_wait_ms(DISPLAY_DELAY_1);

	myHT.displayChar(0,'1', myHT.DecPointOff);
	myHT.displayChar(1,'2', myHT.DecPointOff);
	busy_wait_ms(DISPLAY_DELAY_2);

	myHT.ClearDigits(); //Clear display
	busy_wait_ms(DISPLAY_DELAY_2);

	printf(  "Test single character with dec point on, 4.5.\n");
	myHT.displayChar(0,'4', myHT.DecPointOn);
	myHT.displayChar(1,'5', myHT.DecPointOn);
	busy_wait_ms(DISPLAY_DELAY_2);
	myHT.ClearDigits(); //Clear display
	busy_wait_ms(DISPLAY_DELAY_1);
}


void TestString(void)
{
	printf(  "Test strings\n");
	// Define test strings
	const char *testStrings[] = {
		"1", "AB", "3.4", "-.1", "..", "2.0", "0.5", "-5"
	};
	const size_t numTests = sizeof(testStrings) / sizeof(testStrings[0]);
	// Loop through each test case
	for (size_t i = 0; i < numTests; ++i) {
		// Align Right
		myHT.displayText(testStrings[i], myHT.AlignRight);
		busy_wait_ms(DISPLAY_DELAY_5);
		myHT.ClearDigits();
		// Align Left
		myHT.displayText(testStrings[i], myHT.AlignLeft);
		busy_wait_ms(DISPLAY_DELAY_5);
		myHT.ClearDigits();
	}
	myHT.ClearDigits();
	myHT.displayText(testStrings[3]); // display text can also be called without alignment
	busy_wait_ms(DISPLAY_DELAY_2);
	myHT.ClearDigits();
}

void TestHex(void)
{
	printf(  "Test Hexadecimal\n");
	myHT.displayHexChar(0, '0');
	myHT.displayHexChar(1, '1');
	busy_wait_ms(DISPLAY_DELAY_2); // 012

	myHT.displayHexChar(0, 'A');
	myHT.displayHexChar(1, 'B');
	busy_wait_ms(DISPLAY_DELAY_2); // display ABC
	myHT.ClearDigits();
}

void TestIntPos(void)
{
	printf( "Test positive integers\n");
	// Array of numbers to test
	int32_t testNumbers[] = {1, 34};
	HT16K33plus_model6::TextAlignment_e alignments[] = {myHT.AlignRight, myHT.AlignLeft, myHT.AlignRightZeros};
	// Loop through each number and alignment
	for (int32_t  num : testNumbers)
	{
		for (HT16K33plus_model6::TextAlignment_e align : alignments)
		{
			myHT.displayIntNum(num, align);
			busy_wait_ms(DISPLAY_DELAY_2);
			myHT.ClearDigits();
		}
	}
	myHT.ClearDigits();
}

void TestIntNeg(void)
{
	printf( "Test negative integers\n");
	// Array of numbers to test
	int32_t testNumbers[] = {-2, -6};
	HT16K33plus_model6::TextAlignment_e alignments[] = {myHT.AlignRight, myHT.AlignLeft, myHT.AlignRightZeros};
	// Loop through each number and alignment
	for (int32_t  num : testNumbers)
	{
		for (HT16K33plus_model6::TextAlignment_e align : alignments)
		{
			myHT.displayIntNum(num, align);
			busy_wait_ms(DISPLAY_DELAY_2);
			myHT.ClearDigits();
		}
	}
}

void TestFloat(void)
{
	printf( "Test floating point numbers \n");
	// Array of numbers to test
	float testNumbers[] = {0.5, 1.0, 2.5, 1.39};
	HT16K33plus_model6::TextAlignment_e alignments[] = {myHT.AlignRight, myHT.AlignLeft};
	// Loop through each number and alignment
	for (float  num : testNumbers)
	{
		for (HT16K33plus_model6::TextAlignment_e align : alignments)
		{
			myHT.displayFloatNum(num, align, 1);  // One digit precision
			busy_wait_ms(DISPLAY_DELAY_2);
			myHT.ClearDigits();
		}
	}
	myHT.ClearDigits();
}


void endTest()
{
	myHT.ClearDigits();
	myHT.DisplayOff();
	myHT.Display_I2C_OFF();
	printf( "Test End\n");
}


/// @endcond
