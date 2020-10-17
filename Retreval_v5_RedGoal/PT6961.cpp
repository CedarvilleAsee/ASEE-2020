/*
  PT6961.cpp - Library for communicating with PT6961 LED driver.
  Created by Garrett Blanton January, 16, 2013.
  Released into the public domain.
*/
#include "constants.h"

//0 - A
//1 - B
//2 - C
//3 - D
//4 - E
//5 - F
//6 - G
//7 - Colon

#include "Arduino.h"
#include "PT6961.h"

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
  sendCmd(_DISPLAY_6X12);
  sendCmd(_AUTO_INCREMENT);
  initRAM();
  sendCmd(_DISPLAY_14_16);
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
void PT6961::sendCmd(char cmd)
{
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, cmd);
  digitalWrite(_CS,HIGH);  
}

void PT6961::sendDigit(char digit, char val)
{
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, digit);
  shiftOut(_DIN, _CLK, LSBFIRST, val);
  digitalWrite(_CS,HIGH);    
}

void PT6961::sendNum(int num, char colon)
{
  int digit1 = num / 1000;
  int digit2 = (num % 1000) / 100;
  int digit3 = (num % 100) / 10;
  int digit4 = (num % 10);
  
  sendDigits(digit1,digit2,digit3,digit4,colon);
}

void PT6961::sendDigits(char digit1, char digit2, char digit3, char digit4, char colon)
{

  const char DISP[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x61};
  
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  
  if(colon == 1)
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit1] | 0x80);
  }
  else
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit1]);
  }

  shiftOut(_DIN, _CLK, LSBFIRST, 0xC2);

  if(colon == 1)  
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit2] | 0x80);
  }
  else
  {
    shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit2]);
  }
  
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC4);
  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit3]);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC6);
  shiftOut(_DIN, _CLK, LSBFIRST, DISP[digit4]);
  digitalWrite(_CS,HIGH);    
}

/*void PT6961::sendMessage(int message)
{
  char lineDisp[4];
  if(message == PICKUP_RIGHT) {
    lineDisp[0] = 0x73; //P
    lineDisp[1] = 0x00;
    lineDisp[2] = 0x00;
    lineDisp[3] = 0x50; //r
  }
  else if(message == PICKUP_LEFT) {
    lineDisp[0] = 0x73; //P
    lineDisp[1] = 0x00;
    lineDisp[2] = 0x00;
    lineDisp[3] = 0x38; //L
  }
  else if(message == PICKUP_EMPTY) {
    lineDisp[0] = 0x73; //P
    lineDisp[1] = 0x00;
    lineDisp[2] = 0x00; 
    lineDisp[3] = 0x79; //E
  }
  else if(message == DEPOSITING) {
    lineDisp[0] = 0x5E; //D
    lineDisp[1] = 0x79; //E
    lineDisp[2] = 0x73; //P
    lineDisp[3] = 0x3F; //O
  }
  else if(message == DONE) {
    lineDisp[0] = 0x5E; //D
    lineDisp[1] = 0x3F; //O
    lineDisp[2] = 0x54; //n
    lineDisp[3] = 0x79; //E
  }
  else if(message == RACQUETBALL) {
    lineDisp[0] = 0x7C; //b
    lineDisp[1] = 0x77; //A
    lineDisp[2] = 0x38; //L
    lineDisp[3] = 0x38; //L
  }

  
  
  digitalWrite(_CS,LOW);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC0);
  
  shiftOut(_DIN, _CLK, LSBFIRST, lineDisp[0]);

  shiftOut(_DIN, _CLK, LSBFIRST, 0xC2);

  shiftOut(_DIN, _CLK, LSBFIRST, lineDisp[1]);
  
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC4);
  shiftOut(_DIN, _CLK, LSBFIRST, lineDisp[2]);
  shiftOut(_DIN, _CLK, LSBFIRST, 0xC6);
  shiftOut(_DIN, _CLK, LSBFIRST, lineDisp[3]);
  digitalWrite(_CS,HIGH);    
}*/
