/*
  PT6961.cpp - Library for communicating with PT6961 LED driver.
  Created by Garrett Blanton January, 16, 2013.
  Released into the public domain.
*/
#include "constants.h"
#include "Arduino.h"
#include "PT6961.h"

/*
Description:
The Seven Segment Display hensforth refered to as 7SD opertates as a shift register. The _CS pin must be set low for the 7SD to start listening to the data pin.
Then a code needs to be shifted to the 7SD to tell which digit to write the value. The next code that is shifted to the 7SD will then be writen to the specifed digit.
The code for refernecing the digits are as follows:

  Digit 1: 0xC0
  Digit 2: 0xC2
  Digit 3: 0xC4
  Digit 4: 0xC6

The reason for not using the odd numbers has yet to be discovered.

To write a value to the 7SD the code following the digit code needs to be a hex code where each bit corrisponds to one of the sections of the individual digit the most
significant bit is ignored. The lights are high active. The following graphic shows which bit each section corrisponds to, each letter corrispoinds to a bit of the 8 bit binary number:

  ~gfe dcba   
                  (This diagram is from of the orientation of the small dot light on the bottom)
   a
   _
 f|_|b   center seg = g
 e|_|c
   d

To turn on the colon bit was or (|) with the code 0x80 with the first and second digit. The first digit turns on the top dot of the colon, and the second digit turns on the lower dot.
*/

PT6961::PT6961(int DIN, int CLK, int CS)
{
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  _DIN = DIN;
  _CLK = CLK;
  _CS = CS;
}

void PT6961::initDisplay()
{
  sendCmd_Advanced(_DISPLAY_6X12);
  sendCmd_Advanced(_AUTO_INCREMENT);
  initRAM();
  sendCmd_Advanced(_DISPLAY_14_16);
}

// Initializes RAM to all zeros
void PT6961::initRAM()
{
  //first clear 8 bytes of the display RAM
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  for(int i=0; i<8; i++){
    shiftOut(_DIN, _CLK, LSBFIRST, 0x00);
  }
  digitalWrite(_CS,HIGH); 
}

// Use to send command based on enumeration
void PT6961::sendCmd_Advanced(char cmd)
{
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, cmd);
  digitalWrite(_CS,HIGH);  
}

void PT6961::sendDigit_Advanced(char digit, char val)
{
  //Send the _CS pin Low for the seven seg to being listening to the data pin
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, digit);
  shiftOut(_DIN, _CLK, LSBFIRST, val);
  //Send the Pin high for it to stop listening
  digitalWrite(_CS,HIGH);    
}

void PT6961::sendDigit(unsigned int number, unsigned int index){
  char digit = validateIndex(index);        //Clean input and translate to 7SD codes
  char val   = translateNumber(number);
  
  digitalWrite(_CS, LOW);                   //Send pin low so 7SD begings listening to data pin
  shiftOut(_DIN, _CLK, LSBFIRST, digit);     //Shifts to the least most significant bit to the adress specifed
  shiftOut(_DIN, _CLK, LSBFIRST, val);       //shifts the number to the digit that was speifed in the last shift
  digitalWrite(_CS, HIGH);                  //Send pin low so 7SD stops listening and system resets
  
}

void PT6961::sendNum(unsigned int num, char colon)
{
  int digit1 = num / 1000;
  int digit2 = (num % 1000) / 100;
  int digit3 = (num % 100) / 10;
  int digit4 = (num % 10);
  
  sendDigits(digit1,digit2,digit3,digit4,colon);
}

void PT6961::sendDigits(char digit1, char digit2, char digit3, char digit4, char colon)
{
  
  //const char DISP[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x61};
  
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  
  if(colon == 1)
  {
    shiftOut(_DIN, _CLK, LSBFIRST, _DISP[digit1] | 0x80);
  }
  else
  {
    shiftOut(_DIN, _CLK, LSBFIRST, _DISP[digit1]);
  }

  shiftOut(_DIN, _CLK, LSBFIRST, 0xC2);

  if(colon == 1)  
  {
    shiftOut(_DIN, _CLK, LSBFIRST, _DISP[digit2] | 0x80);
  }
  else
  {
    shiftOut(_DIN, _CLK, LSBFIRST, _DISP[digit2]);
  }
  
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC4);
  shiftOut(_DIN, _CLK, LSBFIRST, _DISP[digit3]);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC6);
  shiftOut(_DIN, _CLK, LSBFIRST, _DISP[digit4]);
  digitalWrite(_CS,HIGH);    
}
char PT6961::validateIndex(unsigned int index){
  if(index > 3){
    return 0xC6;
  }
  else if(index == 2){
    return 0xC4;
  }
  else if(index == 1){
    return 0xC2;  
  }
  else{
    return 0xC0;  
  }
}
char PT6961::translateNumber(unsigned int num){
    int decNumber = num % 10;
    return _DISP[decNumber];
}
char PT6961::translateCharater(char val){
  //TODO
  return val;
}
