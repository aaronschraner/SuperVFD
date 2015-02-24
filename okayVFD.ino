//really big VFD driver
//created by Aaron Schraner
//decided can counter is effectively useless
//repurposed code

#include "chardefs.h"
#include "VFD.h"
#include <limits.h>

#define ONBIT 0x80 //pin 7 (PORTD)

void setup()
{

  DDRB |= 0x3F; //set directions for pins 8-13
  DDRD |= ONBIT; //all we really care about
  
  Serial.begin(9600); //init serial
}


              //01234567
char* dbgmsg = "________";//"d[N] \\ _";
unsigned long long mm;
uint16_t testmask= 0x0080;
byte maskreg;
long myAwesomeInt=200;
void loop()
{
  static VFD* myVFD = new VFD;
  static Message* myMessage = new Message("1337");
  //myVFD->setOrientation(false);
  //myMessage->apply(myVFD);
  //myVFD->setOrientation(true);
  //myMessage->apply(myVFD);
  //myVFD->watch(&myAwesomeInt);
  //static Meter* myMeter = new Meter(&myAwesomeInt,0,8);
  //myVFD->add(myMeter);
  (new Message("SUPERVFD"))->apply(myVFD);
  myVFD->add(new Meter(&myAwesomeInt, 0,Meter::_int4));
  myVFD->setExtra(VFD::_period,1);
  while(true)
  {
    //myAwesomeInt=LONG_MAX;
    myVFD->render(); //all that is required
  }
  

}
