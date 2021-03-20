/*
  PT6961.h - Library for communicating with PT6961 LED driver.
  Created by Garrett Blanton January, 16, 2013.
  Released into the public domain.
*/

#ifndef PT6961_h
#define PT6961_h

#include "Arduino.h"

class PT6961
{
  public:
  
    PT6961(int DIN, int CLK, int CS);
    void initDisplay();
    void initRAM();
    void sendCmd_Advanced(char cmd);                  //sends code comands does not right to display  - Advanced Interface
    void sendDigit_Advanced(char digit, char val);    //writes to a single digit                      - Advanced Interface code comands required

    //All methodes truncate if data is larger than display
    void sendDigit(unsigned int number, unsigned int index = 0);         //writes a single digit number to the display   - simple interface
    void sendNum(unsigned int num, char colon = 0);                     //writes a four digit number to the display     - simple interface
    void sendChar(char charater, unsigned int index);                   //writes a single charater to display           - simple interface
    void sendMessage(int message);                                      //writes a four digit message to the screen     - simple interface

    void sendDigits(char digit1, char digit2, char digit3, char digit4, char colon = 0);
	
    const static char _DISPLAY_6X12 = 0x02;
    const static char _DISPLAY_7X11 = 0x03;
    const static char _AUTO_INCREMENT = 0x40;
    const static char _FIXED_ADDRESS = 0x44;
    const static char _DISPLAY_OFF = 0x80;
    const static char _DISPLAY_1_16 = 0x88;
    const static char _DISPLAY_2_16 = 0x89;
    const static char _DISPLAY_4_16 = 0x8A;
    const static char _DISPLAY_10_16 = 0x8B;
    const static char _DISPLAY_11_16 = 0x8C;
    const static char _DISPLAY_12_16 = 0x8D;
    const static char _DISPLAY_13_16 = 0x8E;
    const static char _DISPLAY_14_16 = 0x8F;
	
  private:
    char validateIndex(unsigned int index);
    char translateNumber(unsigned int num);
    char translateCharater(char val);
    
	  int _DIN;
    int _CLK;
    int _CS;
    const char _DISP[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x61};
	  char _AsciiLookUpTable[26] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E, 0x76, 0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x1D, 0x36, 0x6E, 0x49};
    // Translation:              A      b     C     d     E     F     G     H     I     J     K     L     m     n     O     P     q     r     S     t     U     v     w     X     y     Z
};

#endif
