//#include "MenuItem.h"
/*
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
//        for (int i = 0; i < 8 && ud ; i++)
//        {
//          myMessage[7 - i] = (( results.value >> 4 * i & 0x0F) < 10) ?
//                             ((results.value >> 4 * i & 0x0F) + '0') :
//                             ((results.value >> 4 * i & 0x0F) + 'A' - 10);
//        }
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
*/


//used by nestedMenuLoop//
/*
static Menu rootMenu;
static Menu LEDmenu;
static Menu HDmenu;
static Menu* activeMenu;

class mc
{
  public:
    

    static void EGVup(VFD* vfd) {
      activeMenu = &rootMenu;
    }
    static void EGVopenLEDmenu(VFD* vfd) {
      activeMenu = &LEDmenu;
    }
    static void EGVopenHDmenu(VFD*) {
      activeMenu = &HDmenu;
    }

    static void hd1(VFD*)     {
      HIGHDEL = 1;
    }
    static void hd10(VFD*)    {
      HIGHDEL = 10;
    }
    static void hd100(VFD*)   {
      HIGHDEL = 100;
    }
    static void hd1000(VFD*)  {
      HIGHDEL = 1000;
    }
    static void hd10000(VFD*) {
      HIGHDEL = 10000;
    }


    static void setPow(VFD* vfd)
    {
      static bool powstate = 0;
      powstate = !powstate;
      vfd->setExtra(VFD::_power, powstate);
    }

    static void toggleLamp(VFD* vfd)
    {
      noInterrupts();
      static bool lampState = 0;
      lampState = !lampState;
      vfd->setExtra(VFD::_fan2, lampState);
      if (lampState)
        for (int i = 0; i < 10; i++)
          send_strip(0x00FFFFFF);
      else
        for (int i = 0; i < 10; i++)
          send_strip(0);
      interrupts();
    }

    static void setFan(VFD* vfd)
    {
      static bool powstate = 0;
      powstate = !powstate;
      vfd->setExtra(VFD::_fan1, powstate);
    }

    static void baspingo(VFD* vfd)
    {
      static bool powstate = 0;
      powstate = !powstate;

      for (int i = 0; i < 10; i++)
      {
        vfd->setRaw(i, (powstate ? 0x00C0 | ((i < 8) ? 0x20 : 0) : 0));
      }

    }

    static void orient(VFD* vfd)
    {
      static bool usd = true;
      vfd->setOrientation(usd);
      usd = !usd;
      vfd->reset();
    }

    static void runStrip(VFD* vfd)
    {
      const int nCols = 11;
      static int colN = 0;
      static unsigned long colors[nCols] = {0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000};
      const unsigned long rgb[3] = {0x00FF0000, 0x000000FF, 0x0000FF00};
      static long long col = -1;
      col = (col + 1) % (nCols);
      for (int i = 0; i < nCols; i++)
        colors[i] = 0;
      if (col < nCols - 1)
        colors[col] = rgb[col % 3];

      static Meter* ledMeter = new Meter((long long*)&col, 0, Meter::_int2);
      if (col < nCols - 1)
        vfd->add(ledMeter);
      else
        vfd->del(ledMeter);
      noInterrupts();
      for (int i = 0; i < nCols; i++)
        send_strip(colors[i]);
      interrupts();

    }
};



void MenuUILoop()
{
  static unsigned long LEDtimer = 0;
  char* myMessage = "SUPERVFD";
  int menuPos = 0;
  int maxEntries = 3;

  bool doRender = 1;

  VFD *myVFD = new VFD;
  Message* menuMessage = new Message(myMessage);

  Menu* myMenu = new Menu(myVFD);

  //MenuItem* myItems[3];

  myMenu->add(new MenuItem("POWER", mc::setPow));
  myMenu->add(new MenuItem("FAN", mc::setFan));
  //myMenu->add(new MenuItem("BASPINGO", mc::baspingo));
  myMenu->add(new MenuItem("FLIP SCR", mc::orient));
  myMenu->add(new MenuItem("LEDSTRIP", mc::runStrip));
  myMenu->add(new MenuItem("LAMP", mc::toggleLamp));
  //myMenu->add(new MenuItem("RED", new void(VFD* vfd) { vfd->setExtra(_fan2,1);});
//  myItems[0] = new MenuItem("POWER", setPow);
//  myItems[1] = new MenuItem("FAN", setFan);
//  myItems[2] = new MenuItem("UNKNOWN",});


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

    //noInterrupts();
    if (doRender)
      myVFD->render();
    //interrupts();


  }

}
void nestedMenuLoop()
{
  VFD* myVFD = new VFD;
  rootMenu.setVFD(myVFD);
  LEDmenu.setVFD(myVFD);
  HDmenu.setVFD(myVFD);

  activeMenu = &(rootMenu);
  rootMenu.add(new MenuItem("POWER", mc::setPow));
  rootMenu.add(new MenuItem("FAN", mc::setFan));
  rootMenu.add(new MenuItem("BASPINGO", mc::baspingo));
  rootMenu.add(new MenuItem("FLIP SCR", mc::orient));
  rootMenu.add(new MenuItem("LEDSTRIP", mc::runStrip));
  rootMenu.add(new MenuItem("LAMP", mc::toggleLamp));
  rootMenu.add(new MenuItem("LEDS  >>", mc::EGVopenLEDmenu));
  rootMenu.add(new MenuItem("SPEED >>", mc::EGVopenHDmenu));

  LEDmenu.add(new MenuItem("RED", mc::runStrip));
  LEDmenu.add(new MenuItem("GREEN", mc::runStrip));
  LEDmenu.add(new MenuItem("BLUE", mc::runStrip));
  LEDmenu.add(new MenuItem(" << BACK", mc::EGVup));

  HDmenu.add(new MenuItem("1",     mc::hd1    ));
  HDmenu.add(new MenuItem("10",    mc::hd10   ));
  HDmenu.add(new MenuItem("100",   mc::hd100  ));
  HDmenu.add(new MenuItem("1000",  mc::hd1000 ));
  HDmenu.add(new MenuItem("10000", mc::hd10000));
  HDmenu.add(new MenuItem(" << BACK", mc::EGVup));

  bool doRender = 1;
  char* menuMessage;
  while (true)
  {
    if (irrecv.decode(&results))
    {
      if (results.value != ~0)
      {
        switch (getBtnByCode(results.value))
        {
          case _UP:    activeMenu->goUp(); break;
          case _DOWN:  activeMenu->goDown(); break;
          case _SELECT: activeMenu->select(); break;
          case _POWER:
            doRender = !doRender;
            break;

        }
      }

      irrecv.resume();
    }

    //activeMenu->toStr(myMessage);
    //myItems[menuPos]->toStr(myMessage);
    //menuMessage->set(myMessage);

    //menuMessage->apply(myVFD);
    menuMessage = activeMenu->activeItem()->getLabel();
    {
      int i;
      for (i = 0; i < 8; i++)
        if (menuMessage[i])
          myVFD->setChar(i, menuMessage[i]);
        else
          break;
      for (i; i < 8; i++)
        myVFD->setChar(i, ' ');
    }
    //noInterrupts();
    if (doRender)
      myVFD->render();
    //interrupts();


  }
}
*/

