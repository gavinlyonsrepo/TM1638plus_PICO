/*
* Project Name: TM1638plus_Model2 
* File: TM1638plus_Model2.h
* Description: TM1638plus_Model2.h header file Arduino library for Model 2 TM1638 module(16 KEY 16 pushbuttons).
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/TM1638plus_PICO
*/


#ifndef TM1638PLUS_MODEL2_H
#define TM1638PLUS_MODEL2_H

#include "pico/stdlib.h"
#include <cstdio>
#include <cstring>
#include "tm1638plus_common.h"
 
class TM1638plus_Model2 : public TM1638plus_common {

public:
    // Constructor Init the module
    // parameters 
    // 1. strobe = GPIO connected to strobe line of module
    // 2. clock = GPIO connected to clock line of module
    // 3. data = GPIO connected to data line of module
    // 4. swap_nibbles = boolean default false, if true swaps nibbles on display byte

    TM1638plus_Model2(uint8_t strobe, uint8_t clock, uint8_t data, bool swap_nibbles);
    
    // Methods
    
    // Function Desc:Read push buttons function 
    // User may have to deb-ounce buttons depending on application.
    // See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
    // Note there are two different read button functions See Also ReadKeyTwo
    // Returns: zero if no button pressed.  
    // Returns: a byte with decimal value of buttons 1-16 (1 for S1 2 for S2 ... 16 for S16 ).
   uint8_t ReadKey16(void);

    // Function Desc: Reads the push  buttons:  alternate function to ReadKeys16
    // Can detect multiple key presses , See notes section in readme regarding
    // Problems with seven segment display when pressing certain keys in combination
    // Returns: A 16 bit integer where each bit correpsond to a switch
    // Eg:
    // S1 = 0x0001
    // S16 = 0x8000
    // S16 + S1 together = 0x8001
    uint16_t ReadKey16Two(void);
    
    // Send seven segment value module
    // pass segment  0-7, byte of data corresponding to segments abcdefg(dp) 01234567.
    // a is 0 , dp is 7 , segmentValue is which segments are off or on for each digit. d7d6d5d54d3d2d1 
    // Have to call it 8 times in row to set all segments. 
    // to set all "a" on send (0x00,0xFF)
    // to set all segment "g" off (0x06,0X00)
    void DisplaySegments(uint8_t segment, uint8_t segmentValue);
    
    // Display a Hexadecimal number ,takes a number and byte for decimal point display, leading zeros optional
    // converts to string. 
    //void DisplayHexNum(unsigned long number, byte dots, boolean leadingZeros = true);
    void DisplayHexNum(uint16_t  numberUpper, uint16_t numberLower, uint8_t dots, bool leadingZeros = true);
    
    // Display a decimal number , takes a number and byte for decimal point display,  leading zeros optional 
    // converts to string. 
    void DisplayDecNum(unsigned long number, uint8_t dots, bool leadingZeros = true);

    // Display a string , takes a string and byte for decimal point display
    // Takes in string , converts it to ASCII using the font and masks for the decimal point
    // Then passes array of eight ASCII bytes to DisplayValues function
    void DisplayStr(const char* string, const uint16_t dots = 0);
   
    // Takes in Array of 8 ASCII bytes , Called from DisplayStr 
    // Scans each ASCII byte converts to array of 8 segment bytes where each byte represents a segment
    // i.e. byte 0 represents a in segment and then each bit represents the a segment in each digit.
    // So for "00000005" is converted by DisplayStr to hex "3F 3F 3F 3F 3F 3F 3F 6D" where left is first digit.
    // this is converted to hex "FF FE FF FF FE FF 01 00"  Where left is first segment
    // So "a" segment is turned on for all bytes and "b" is on for all except last digit.
    // The bits are  mapping below abcdefg(dp) = 01234567 ! 
    // see for mapping of seven segment to digit https://en.wikipedia.org/wiki/Seven-segment_display
    void ASCIItoSegment(const uint8_t values[]);
    
    //Divides the display into two nibbles and displays a Decimal number in each.
    //takes in two numbers 0-9999 for each nibble ,  and byte for decimal point display,
    //and leading zeros optional
    void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, bool leadingZeros = true);
        
private:  
        bool _SWAP_NIBBLES= false;   

};
 
#endif
