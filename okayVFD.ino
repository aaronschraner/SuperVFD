//really big VFD driver
//created by Aaron Schraner
//decided can counter is effectively useless
//repurposed code

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
#define UNO
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define MEGA
#endif

#include "chardefs.h"
#include "VFD.h"
#include "RemoteHandler.h"
#include "debug.h"
#ifdef USESD
#include <SPI.h>
#include <SD.h>
#endif
//#include "Menu.h"
//#include "MenuItem.h"
#include <limits.h>
#include <IRremote.h>
#include <EEPROM.h>
#define USESERIAL
//#define USETIMER
//#define DEBUG
#define ADDR_WORD 0x16

//saveWord: save a c-string to EEPROM
void saveWord(char* w)
{
  for (int i = 0; i < 8; i++)
  {
    EEPROM.write(ADDR_WORD + i, w[i]);
  }
}

//loadWord: load c-string into w from EEPROM
int loadWord(char* w)
{
  bool hadString = 0;
  for (int i = 0; i < 8; i++)
  {
    hadString |= EEPROM.read(ADDR_WORD + i);
  }
  if (!hadString)
    return -1;
  else
  {
    for (int i = 0; i < 8; i++)
    {
      w[i] = EEPROM.read(ADDR_WORD + i);
    }
    return 0;
  }
}

int IRin = A5; //pin that IR reciever is connected to
IRrecv irrecv(IRin); //always uses pin 2 

decode_results results; //for recieving IR codes

void flashDisplay()
{
  VFD myVFD;
  const int ondel=1000;
  while(millis()<ondel*1/2)
  {
    int toSet=millis()*((int)VFD::_period+1)/(ondel*3/4);
    myVFD.setMessage("OKAYVFD");
    myVFD.setExtra((VFD::ExtraLed)(toSet%((int)VFD::_period)+1), 1);
    myVFD.render();
    myVFD.setExtra((VFD::ExtraLed)(toSet%((int)VFD::_period)+1), 0);
  }
  while(millis() < ondel*5/8)
  {
    switch( (millis() - ondel * 4 / 8) * 32 / ondel)
    {
      case 0:          //0123456789ABCDEF
        myVFD.setRaw(8,0b1000000000111111);
        myVFD.render();
        myVFD.setRaw(8,0);
        break;
      case 1:          //0123456789ABCDEF
        myVFD.setRaw(8,0b0111111100000000);
        myVFD.render();
        myVFD.setRaw(8,0);
        break;
      case 2:          //0123456789ABCDEF
        myVFD.setRaw(9,0b1000000000111111);
        myVFD.render();
        myVFD.setRaw(9,0);
        break;
      case 3:          //0123456789ABCDEF
        myVFD.setRaw(9,0b0111111100000000);
        myVFD.render();
        myVFD.setRaw(9,0);
        break;
        default:
        break;
    }
  }
  myVFD.setRaw(9,0b1111111100111111);
  myVFD.setRaw(8,0b1111111100111111);
  for(int i=0;i<=VFD::_period; i++)
  {
    myVFD.setExtra((VFD::ExtraLed)i,1);
  }
  while(millis()<ondel)
    myVFD.render();
}

void setup()
{
  //reset_strip();
#ifdef UNO
  DDRC |= 0x01;
  DDRD|=0xFC;
  DDRB|=0x1F;
  /*pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);*/
  //pinMode(11,OUTPUT);
  //pinMode(12,OUTPUT);
  //pinMode(13,OUTPUT);
#elif defined(MEGA)

#endif

#ifdef USESERIAL
Serial.begin(9600); //init serial
#endif
  irrecv.enableIRIn(); //enable IR reciever
  flashDisplay();
}

void loop()
{
  //TextEdLoop(); //text editor
  //MenuUILoop(); //uhh
  //nestedMenuLoop(); //attempt (may not work)
  //remoteLoop(); //first attempt at remote control (probably broken)
  
  
  remoteLoop2(); //control TV with VFD + remote interface (works as of 3/29/15)
  //IRrecLoop(); //print a file from SD card 
  //fGenLoop(); //simple square function generator (accurate to up to 2kHz, works 3/29/15, requires USETIMER)
  //analogReaderLoop();//show some analog read values
  //splashLoop("NAILEDIT");//display this string
  //ser2dispLoop();//display serial input
}
