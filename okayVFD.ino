//really big VFD driver
//created by Aaron Schraner
//decided can counter is effectively useless
//repurposed code
#define TKD2 A0
#include "chardefs.h"
#include "VFD.h"
#include "RemoteHandler.h"
#include "debug.h"
//#include "Menu.h"
//#include "MenuItem.h"
#include <limits.h>
#include <IRremote.h>
#include <EEPROM.h>
//#define DEBUG
#define ADDR_WORD 0x16

void saveWord(char* w)
{
  for(int i=0;i<8;i++)
  {
    EEPROM.write(ADDR_WORD+i, w[i]);
  }
}

int loadWord(char* w)
{
  bool hadString=0;
  for(int i=0;i<8;i++)
  {
    hadString |= EEPROM.read(ADDR_WORD+i);
  }
  if(!hadString)
    return -1;
  else
  {
    for(int i=0;i<8;i++)
    {
      w[i]= EEPROM.read(ADDR_WORD+i);
    }
    return 0;
  }
}

int IRin = 6;
IRrecv irrecv(IRin);

decode_results results;

#define ONBIT 0x80 //pin 7 (PORTD)

void setup()
{
  DDRC=0x01;
  reset_strip();

  DDRB |= 0x3F; //set directions for pins 8-13
  DDRD |= ONBIT; //all we really care about

  Serial.begin(9600); //init serial
  
  irrecv.enableIRIn();
  Serial.println("\n\n");
  debug("nothing");
  debug("nothing");
  debug("nothing");
  debug("nothing");
    Serial.println("\n\n");
}

void loop()
{
//TextEdLoop();
//MenuUILoop();
//nestedMenuLoop();
//remoteLoop();
remoteLoop2();
}
