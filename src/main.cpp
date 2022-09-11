/*
  Project Name: TM1638
  File: TM1638plus_TEST_Model2.ino
  Description: demo file library for "model 2" TM1638 module(16 KEY 16 pushbuutons).
  Carries out series of tests demonstrating PICO library TM1638plus_PICO.
  The tests will increment automatically with exception of test9, to enter press S16 during test8
  
  TESTS

  TEST0 = reset function test  
  TEST1 =  decimal numbers
  TEST2 =  Hexadecimal number
  TEST3 = manually set segments 
  TEST4 = Display  strings
  TEST5  =  ASCII to segments ( no reference to font table)
  TEST6 = Brightness control 
  TEST7 = Scroll text example
  TEST8 = Push buttons ReadKey16() buttons function , press 16 to goto test9
  TEST9 = Push buttons ReadKeys16Two() alternate  buttons function
  
  Author: Gavin Lyons.
  URL: https://github.com/gavinlyonsrepo/TM1638plus_PICO
*/

#include "pico/stdlib.h"
#include "tm1638/tm1638plus_model2.h" //include the library

// GPIO I/O pins on the PICO connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM 4  // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 3  // clock = GPIO connected to clock line of module
#define  DIO_TM 2 // data = GPIO connected to data line of module
bool swap_nibbles = false; //Default is false if left out, see issues section in readme at URL
 
// Constructor object Init the module
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles);
// For test setup
#define  myTestDelay 5000
#define  myTestDelay1 1000
#define  myTestDelay2 250
uint8_t  testcount = 0;


void Test0(void);
void Test1(void);
void Test2(void);
void Test3(void);
void Test4(void);
void Test5(void);
void Test6(void);
void Test7(void);
void Test8(void);
void Test9(void);


int main() {
  
  tm.displayBegin(); // Init the module
  busy_wait_ms(myTestDelay2);
  // Test 0 reset test
  tm.reset();

  while(1)
  {
    testcount++;
    switch (testcount)
    {
      case 1: Test1(); break; // Test 1 decimal numbers
      case 2: Test2(); break; // Test 2 Hexadecimal number
      case 3: Test3(); break; // Test 3a 3b & 3C using DisplaySegments method
      case 4: Test4(); break; // Test 4 strings
      case 5: Test5(); break; // Test 5 ASCIItoSegment method 
      case 6: Test6(); break; // Test 6  Brightness and reset
      case 7: Test7(); break; // Test 7 scroll text
      case 8: Test8(); break; // Test 8 Buttons , ReadKey16() returns byte 1-16 decimal, press S16 to goto test9
      case 9: Test9(); break; // Test 9 Buttons , Readkey16Two() alternate buttons function. 
    }
  }
}


void Test1(void)
{
  // Test 1 decimal numbers
  tm.DisplayDecNum(250, 1 << 2, true); // 000002.50
  busy_wait_ms(myTestDelay);
  tm.DisplayDecNum(99991111, 1 << 4, true); // 9999.1111
  busy_wait_ms(myTestDelay);
  tm.DisplayDecNum(2888, 0 , true);  // 00002888
  busy_wait_ms(myTestDelay);
  tm.DisplayDecNum(331285, 1 <<4 ,false); // "3312.85   "
  busy_wait_ms(myTestDelay);
  tm.DisplayDecNum(-33, 0 , false); // "-33        "
  busy_wait_ms(myTestDelay);
  // Test 1b  decimal numbers with the DisplayDecNumNibble function divides display into two nibbles.
  tm.DisplayDecNumNibble(2134 , 78, 1<<4 , true); // "2134.0078"
  busy_wait_ms(myTestDelay);
}

void Test2(void)
{
  // Test 2 Hexadecimal number
  tm.DisplayHexNum(0x0000, 0x456E, 0x00, true); // 0000456E
  busy_wait_ms(myTestDelay);
  tm.DisplayHexNum(0xABCD, 0xEF23, 0x00, true); // ABCDEF23
  busy_wait_ms(myTestDelay);
  tm.DisplayHexNum(0x0000, 0x00FF, 1 << 4); // 0000.00FF
  busy_wait_ms(myTestDelay);
}

void Test3(void)
{
  // Test 3 manually set segments abcdefg(dp) = 01234567
  // display a one in position one "       1"
  tm.DisplaySegments(0, 0x00); //a
  tm.DisplaySegments(1, 0x01); //b, for b turn on digit one only
  tm.DisplaySegments(2, 0x01); //c, for c turn on digit one only
  tm.DisplaySegments(3, 0x00); //d
  tm.DisplaySegments(4, 0x00); //e
  tm.DisplaySegments(5, 0x00); //f
  tm.DisplaySegments(6, 0x00); //g
  tm.DisplaySegments(7, 0x00); //DP
  busy_wait_ms(myTestDelay);

  // Test 3b manually set segments
  // Display "00000005"
  tm.DisplaySegments(0, 0xFF); //a, turn a on for all digits
  tm.DisplaySegments(1, 0xFE); //b
  tm.DisplaySegments(2, 0xFF); //c
  tm.DisplaySegments(3, 0xFF); //d
  tm.DisplaySegments(4, 0xFE); //e
  tm.DisplaySegments(5, 0xFF); //f
  tm.DisplaySegments(6, 0x01); //g , for g middle segment, digit one only on
  tm.DisplaySegments(7, 0x00); //decimal point, turn off all decmial points
  busy_wait_ms(myTestDelay);

  // Test 3c manually set segments scroll g 
  // Display "-" countup to "--------"
  tm.reset();
  uint8_t dashvalue = 1;
  for (uint8_t j = 0; j < 8;  j++)
  {
    
    tm.DisplaySegments(6, dashvalue); // g scrolling 
    dashvalue = (dashvalue*2)+1; // 1 to 256
    busy_wait_ms(myTestDelay1);
  }
  busy_wait_ms(myTestDelay);

}

void Test4(void)
{
  // Test 4 strings
  tm.DisplayStr("helloYOU", 1); // "helloYOU."
  busy_wait_ms(myTestDelay);
  tm.DisplayStr("      Hi", 0x08); // "     . Hi"
  busy_wait_ms(myTestDelay);
  tm.DisplayStr("   ---   ", 0XE7); // ". . .---. . ."
  busy_wait_ms(myTestDelay);
  tm.DisplayStr(" helloU2", 0); // " helloU2"
  busy_wait_ms(myTestDelay);
  tm.DisplayStr("hello", 0);  // "hello   "
  busy_wait_ms(myTestDelay);
}

void Test5(void)
{
  // Test 5 ASCII to segments takes an array of bytes and displays them
  // without ref to the ASCII font table direct data to digits to displays 3F 3F 3F 6D 3F 3F 3F 6D = 00050005
  // gfedcba = 3F for zero https://en.wikipedia.org/wiki/Seven-segment_display
  
  const uint8_t values[8] = {0x3F, 0x3F, 0x3F, 0x6D, 0x3F, 0x3F, 0x3F, 0x6D}; // for ascii to segment test 00050005
  
  tm.ASCIItoSegment(values);
  
  busy_wait_ms(myTestDelay);
}

void Test6(void)
{
  // Test 6  Brightness and reset
  for (uint8_t brightness = 0; brightness < 8; brightness++)
  {
    tm.brightness(brightness);
    tm.DisplayStr("brightnes", 1);
    busy_wait_ms(myTestDelay1);
  }
  tm.brightness(2); 
  tm.reset();
}

// Just one possible method to scroll text there are many others.
void Test7(void)
{
  char textScroll[17] = " Hello world 123";
  unsigned long previousMillis_display = 0;  // will store last time display was updated
  const long interval_display = 1000;            //   interval at which to update display (milliseconds)

  while(1)
  {
  tm.DisplayStr(textScroll, 0);
  unsigned long currentMillis = to_ms_since_boot(get_absolute_time());;
  //  update data every interval_display delay
  if (currentMillis - previousMillis_display >= interval_display)
  {
    previousMillis_display = currentMillis;
    if (strlen(textScroll) > 0)
    {
      memmove(textScroll, textScroll+1, strlen(textScroll)); // delete first char in array.
    }else
    {
      return;
    }
   }
  }
}

void Test8(void)
{
  unsigned char buttons;
  while(1)
  {
      // Test 8 , buttons readkey16() function, no debounce see notes at URL for example to debounce.
      // returns 0-16 , 0 for nothing pressed.
      // NOTE: pressing  S16 will move to test 9 
      buttons = tm.ReadKey16();
      tm.DisplayDecNum(buttons, 0 ,false); 
      busy_wait_ms( myTestDelay2);
      if (buttons == 16)
      {
           //pressing 16 moves  to test 9
          busy_wait_ms( myTestDelay2);
          return;
      }
    }
}

//returns word with binary value of switch. S16 = Bit 15 , S15 = bit 14 etc
void Test9(void)
{
  uint16_t buttons=0;
  tm.DisplayStr("buttons2", 0);
  busy_wait_ms( myTestDelay2);
  while(1) 
      {
        // Test 9 buttons Read_key16_two() function
        // returns a uint16_t where each bit represents a switch.
        // S16S15S14S13S12S11S10S9S8S7S6S5S4S3SS2S1. 
        // eg S1 =  0x0001
        // eg S16 = 0x8000
        // eg S1 + S16 = 0x8001     
        // Can be used to detect multi key presses , see REadme.
        // For issues related to display when pressing multi keys together. 
        buttons = tm.ReadKey16Two();
        tm.DisplayHexNum(0x0000, buttons, 0x00, true); 
        busy_wait_ms( myTestDelay2);
      }
}
