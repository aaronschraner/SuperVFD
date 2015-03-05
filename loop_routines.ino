#include "MenuItem.h"

void TextEdLoop()
{
  char* myMessage = "Hello!  ";
  loadWord(myMessage);
  static VFD* myVFD = new VFD;
  //initialize VFD objects
  Message* IRcode = new Message(myMessage);
  int del = 0;
  int cursorPos = 0;
  bool ud = true;
  bool doRender = true;
  //RemoteHandler myRH;
  IRcode->apply(myVFD);

  while (true)
  {
    if (irrecv.decode(&results))
    {
      if (results.value != ~0)
      {
        //Serial.println(results.value, HEX);
        /*for (int i = 0; i < 8 && ud ; i++)
        {
          myMessage[7 - i] = (( results.value >> 4 * i & 0x0F) < 10) ?
                             ((results.value >> 4 * i & 0x0F) + '0') :
                             ((results.value >> 4 * i & 0x0F) + 'A' - 10);
        }*/
        switch (getBtnByCode(results.value))
        {
          case _UP:    myMessage[cursorPos]++; break;
          case _LEFT:  cursorPos = (7 + cursorPos) % 8; break;
          case _RIGHT: cursorPos = (1 + cursorPos) % 8; break;
          case _DOWN:  myMessage[cursorPos]--; break;
          case _SELECT: ud = !ud; break;
          case _A:     myMessage[cursorPos] = 'A'; break;
          case _B:     myMessage[cursorPos] = 'a'; break;
          case _C:     myMessage[cursorPos] = ' '; break;
          case _POWER:
            doRender = !doRender;
            if (!doRender)
              saveWord(myMessage);

            break;

        }
        myVFD->setExtra(VFD::_power, !ud);
        IRcode->set(myMessage);
        IRcode->apply(myVFD);

      }
      irrecv.resume();

    }
    if (!ud)
    {
      myVFD->setChar(cursorPos, (millis() % 800 < 400) ? myMessage[cursorPos] : '_');
    }
    //handle live updating

    if (Serial.available())
    {
      int i = 0;
      while (Serial.available() && i < 8)
      {
        myMessage[i] = Serial.read();
        i++;
        delayMicroseconds(1000);
      }
      myMessage[i] = 0;
      IRcode->apply(myVFD);
    }
    if (doRender)
      myVFD->render(); //all that is required
  }


}

void setPow(VFD* vfd)
{
  static bool powstate = 0;
  powstate = !powstate;
  vfd->setExtra(VFD::_power, powstate);
}


void setFan(VFD* vfd)
{
  static bool powstate = 0;
  powstate = !powstate;
  vfd->setExtra(VFD::_fan1, powstate);
}

void baspingo(VFD* vfd)
{
  static bool powstate = 0;
  powstate = !powstate;

  for (int i = 0; i < 10; i++)
  {
    vfd->setRaw(i, (powstate ? 0x00C0|((i<8)?0x20:0) : 0));
  }

}

void orient(VFD* vfd)
{
  static bool usd=true;
  vfd->setOrientation(usd);
  usd=!usd;
  vfd->reset();
}

void runStrip(VFD* vfd)
{
  const int nCols=11;
  static int colN=0;
  static unsigned long colors[nCols]={0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000};
  const unsigned long rgb[3]={0x00FF0000,0x000000FF,0x0000FF00};
  static long long col=-1;
  col=(col+1)%(nCols);
  for(int i=0;i<nCols;i++)
    colors[i]=0;
  if(col<nCols-1)
    colors[col]=rgb[col%3];
  
  static Meter* ledMeter=new Meter((long long*)&col,0,Meter::_int2);
  if(col<nCols-1)
    vfd->add(ledMeter);
  else
    vfd->del(ledMeter);
  //LEDmessage->set(messages[col]);
  //LEDmessage->apply(vfd);
  noInterrupts();
  /*switch(col)
  {
    case 0:
      for(int i=0;i<10;i++)
                   //__RRBBGG
        send_strip(0x000000FF);
        break;
    case 1:
      for(int i=0;i<10;i++)
        send_strip(0x0000FF00);
        break;
    case 2:
      for(int i=0; i<10; i++)
        send_strip(0x00FF0000);
        break;
    case 3:
      for(int i=0;i<10;i++)
        send_strip(0);
        break;
  }*/
  for(int i=0;i<nCols;i++)
    send_strip(colors[i]);
  interrupts();
  
}
void MenuUILoop()
{
  static unsigned long LEDtimer=0;
  char* myMessage = "SUPERVFD";
  int menuPos = 0;
  int maxEntries = 3;

  bool doRender = 1;

  VFD *myVFD = new VFD;
  Message* menuMessage = new Message(myMessage);

  Menu* myMenu = new Menu(myVFD);

  //MenuItem* myItems[3];

  myMenu->add(new MenuItem("POWER", setPow));
  myMenu->add(new MenuItem("FAN", setFan));
  myMenu->add(new MenuItem("BASPINGO", baspingo));
  myMenu->add(new MenuItem("FLIP SCR", orient));
  myMenu->add(new MenuItem("LEDSTRIP", runStrip));
  /*myItems[0] = new MenuItem("POWER", setPow);
  myItems[1] = new MenuItem("FAN", setFan);
  myItems[2] = new MenuItem("UNKNOWN",});*/


  while (true)
  {
    if (irrecv.decode(&results))
    {
      if (results.value != ~0)
      {
        switch (getBtnByCode(results.value))
        {
          case _UP:    myMenu->goUp(); break;
          case _DOWN:  myMenu->goDown(); break;
          case _SELECT: myMenu->select(); break;
          case _POWER:
            doRender = !doRender;
            break;

        }
      }

      irrecv.resume();
    }

    myMenu->toStr(myMessage);
    //myItems[menuPos]->toStr(myMessage);
    menuMessage->set(myMessage);

    menuMessage->apply(myVFD);

    if (doRender)
      myVFD->render();
      
    
  }

}

