#include "Menu.h"
#include "actions.h"
SC* sonyCodes;

typedef char pgm;

#ifdef DEBUG
void debug(char* action, bool ending)
{
#ifdef USESERIAL
  if (ending)
  {
    Serial.print("##DEBUG##: ");
    Serial.print(action);
    Serial.println(" terminated");
    printDynMem();
  }
  else
  {
    printDynMem();
    printDynMem();
    Serial.print("##DEBUG##: ");
    Serial.print(action);
    Serial.println(" called");
  }
#endif
}
void debug(char* action)
{
  debug(action, 0);
}
void ndbug(char* action)
{
  debug(action, 1);
}
#else
#define debug(x)
#define ndbug(x)
#endif

void printDynMem()
{
#ifdef USESERIAL
  byte* b = new byte;
  Serial.print((int)b);
  Serial.println(" bytes of dynamic memory used");
  /*#ifdef DEBUG
    Serial.print("##DEBUG##: ");
  #endif*/
  //Serial.print((int)(&b));
  //Serial.println(" bytes of stack used");
#ifndef DEBUG
  Serial.print('\n');
#endif
  //return (int)b;
  free(b);
#endif
}


/*
class MenuItem
{
  private:
    void* param = 0;

  public:
    void (*select)(void*) = 0;
    char* label = 0;
    MenuItem(char* label, void (*select)(void*), void* param)
    {
      this->select = select;
      this->label = label;
      this->param = param;
    }
    void setLabel(char* label)
    {
      this->label = label;
    }
    void execute()
    {
      select(param);
    }
};

class Menu
{
  private:
    MenuItem* items[10];
    unsigned int optionCount;
  public:
    int add(MenuItem* mi)
    {
      if (optionCount < 10)
        items[optionCount++] = mi;
      return optionCount - 1;
    }
    void del(MenuItem* mi)
    {
      for (int i = 0; i < optionCount; i++)
        if (items[i] = mi)
        {
          delete items[i];
          items[i] = 0;
        }
    }

    MenuItem* at(unsigned int i)
    {
      return items[i];
    }

    unsigned int getItemCount()
    {
      return optionCount;
    }
};
*/
class myParamBlock
{
  public:
    char* str;
    bool fan;
    VFD* vfd;
    myParamBlock(char* str, bool fan, VFD* vfd)
    {
      this->fan = fan;
      this->str = str;
      this->vfd = vfd;
    }
};

void myFN(void* pblock)
{
  myParamBlock* pb = (myParamBlock*)pblock;
  int i;
  for (i = 0; i < 8 && pb->str[i]; i++)
    pb->vfd->setChar(i, pb->str[i]);
  for (i; i < 8; i++)
    pb->vfd->setChar(i, ' ');

  pb->vfd->setExtra(VFD::_fan1, pb->fan);
#ifdef USESERIAL
  Serial.println("A");
#endif
}

void myIRhandler(unsigned long code, void* param)
{
  if (code != ~0)
  {
#ifdef USESERIAL
    Serial.println(code, HEX);
#endif
    myParamBlock* pb = (myParamBlock*)(param);
    for (int i = 0; i < 8 ; i++)
    {
      pb->vfd->setChar(7 - i, ((code >> 4 * i & 0x0F) < 10 ) ?
                       ((code >> 4 * i & 0x0F) + '0') :
                       ((code >> 4 * i & 0x0F) + 'A' - 10));
    }
  }
}
/*
class MenuViewer
{
  private:
    Menu* menu;
    int pos = 0;
    int items;
    MenuItem* currentItem;
  public:
    void renderTo(VFD* vfd)
    {
      int i;

      currentItem = menu->at(pos);
      for (i = 0; i < 8 && currentItem->label[i]; i++)
        vfd->setChar(i, currentItem->label[i]);
      for (i; i < 8; i++)
        vfd->setChar(i, ' ');

      //vfd->setExtra(VFD::_fan2,(currentItem->type==MenuItem::_menu));
    }
    void setListeners(Screen* screen)
    {
      screen->bindButton(_UP, &(MenuViewer::goUp), this);
    }
    MenuViewer(Menu* menu)
    {
      this->menu = menu;
      this->items = menu->getItemCount();
      currentItem = menu->at(pos);

    }
    void goUp()
    {
      pos = (pos + 1) % menu->getItemCount();
      currentItem = menu->at(pos);

      Serial.println("Going up");
      Serial.println(pos);
    }
    void goDown()
    {
      pos = (pos + menu->getItemCount() - 1) % menu->getItemCount();
      currentItem = menu->at(pos);
      Serial.println("Going down");
      Serial.println(pos);
    }
    static void select(void* param)
    {
      ((MenuViewer*)param)->menu->at(((MenuViewer*)param)->pos)->execute();
    }

    static void goUp(void* param)
    {
      Serial.println("GO UP");
      ((MenuViewer*)param)->goUp();
    }
    static void goDown(void* param)
    {
      Serial.println("GO DOWN");
      ((MenuViewer*)param)->goDown();
    }

};
*/
void toggleFan(void* vfd)
{

  static bool fanState = 0;
#ifdef USESERIAL
  Serial.println("HAXED");
#endif
  fanState = !fanState;
  ((VFD*)vfd)->setExtra(VFD::_fan1, fanState);
}
void orient(VFD* vfd)
{
  vfd->setOrientation(!vfd->getOrientation());
  //vfd->reset();
}
/*
void IRhandle(unsigned long code, void* param)
{
  Serial.println(code, HEX);
  static remoteBtn lastAction;
  switch (code)
  {
    case 0x10EFA05F:
      lastAction = _UP;
      (*((int*)param))++;
      break;
    case 0x10EF00FF:
      (*((int*)param))--;
      lastAction = _DOWN;
      break;
      default:
        if(lastAction==_DOWN)
        {
          (*((int*)param))--;
        }
        else if(lastAction==_UP)
        {
          (*((int*)param))++;
        }
        break;

  }
}
*/

bool doRender = 0;

void toggleRender(void*)
{
  doRender = !doRender;
}

void remoteLoop()
{
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  myParamBlock* mb = new myParamBlock("Ayy", 1, myVFD);
  //int myInt;

  //myScreen.bindIRCode(0xA90, toggleFan, myVFD);
  myScreen.bindIRHandler(myIRhandler, mb);

  bool doRender = 1;


  while (true)
  {
    myScreen.render();


    //myVFD->setNum(0, myInt);
    if (doRender)
      myVFD->render();
  }
}


void testVoid(VFD* vfd)
{
  vfd->setExtra(VFD::_fan1, 1);
}

class MN //menu navigator
{
  public:
    static void goUp( void* menu)
    {
      ((Menu*)menu)->goUp();
    }
    static void goDown( void* menu)
    {
      ((Menu*)menu)->goDown();
    }
    static void select(void* menu)
    {
      ((Menu*)menu)->select();
    }
    static void bindMN(Screen* screen, Menu* menu)
    {

      screen->bindButton(_UP, MN::goUp, menu);
      screen->bindButton(_DOWN, MN::goDown, menu);
      screen->bindButton(_SELECT, MN::select, menu);
      screen->bindButton(_RIGHT, MN::select, menu);
      screen->bindButton(_POWER, toggleRender, 0);

    }

};

class Menupacket
{
  public:
    Menu** activeMenu;
    Menu* targetMenu;
    Screen *screen = 0;
    //Menupacket(Menu** acm, Menu* tgm): activeMenu(acm), targetMenu(tgm) {}
    Menupacket(Menu** acm, Menu* tgm, Screen* scr): activeMenu(acm), targetMenu(tgm), screen(scr) {}

    static void menuOpen(void* packet) {
      Menupacket* pkt = (Menupacket*)packet;
      if (pkt->screen)
      {
        //pkt->targetMenu = *(pkt->activeMenu);
        //swap target menu and *active menu
        Menu* tempMenu = *(pkt->activeMenu);
        pkt->screen->bindButton(_LEFT, Menupacket::menuOpen, packet);
        MN::bindMN(((Menupacket*)packet)->screen, ((Menupacket*)packet)->targetMenu);
        *(pkt->activeMenu) = pkt->targetMenu;
        pkt->targetMenu = tempMenu;
      }


    }
    static void menuOpen(VFD* vfd, void* packet) {
      menuOpen(packet);
    }
};


class sonypacket
{
  public:
    Screen* screen;
    Menu** activeMenu;
    VFD* vfd;
    sonypacket(Screen* scr, Menu** acm, VFD* vfd): screen(scr), activeMenu(acm), vfd(vfd) {

    }
};
void unbindSonyNav(void* scr)
{
  sonypacket* pkt = (sonypacket*)scr;
  pkt->screen->unbindAllButtons();
  MN::bindMN(pkt->screen, *(pkt->activeMenu));
  pkt->vfd->setExtra(VFD::_hand, 0);
  //pkt->screen->bindButton(_LEFT, Menupacket::menuOpen, new Menupacket((pkt->activeMenu), *(pkt->activeMenu), pkt->screen));
}
void bindSonyNav(VFD*, void* scr)
{
  sonypacket* sp = (sonypacket*)scr;
  sp->vfd->setExtra(VFD::_hand, 1);
  sp->screen->bindButton(_UP,     IRaction::sendCode, sonyCodes->up);
  sp->screen->bindButton(_DOWN,   IRaction::sendCode, sonyCodes->down);
  sp->screen->bindButton(_RIGHT,  IRaction::sendCode, sonyCodes->right);
  sp->screen->bindButton(_LEFT,   IRaction::sendCode, sonyCodes->left);
  sp->screen->bindButton(_SELECT, IRaction::sendCode, sonyCodes->select);
  sp->screen->bindButton(_A,      unbindSonyNav, scr);
  sp->screen->bindButton(_B,      IRaction::sendCode, sonyCodes->power);

}
//CONDEMNED
//Massive memory leaks and inefficiency.
//Will eventually rewrite.

void printDynMem(VFD* vfd)
{
  byte* b = new byte;
  vfd->setNum(0, ((int)b / 100) % 10);
  vfd->setNum(1, (int)b % 100);
  free(b);
}

void memctl(VFD* vfd, void* packet)
{
  static byte* allocs[10];
  static int alloct = 0;
  if (*((char*)packet) == '+' && alloct < 10)
  {
    allocs[alloct++] = new byte;
  }
  else if (*((char*)packet)  == '-' && alloct > 0)
  {
    free(allocs[--alloct]);
  }
}

const char labels[][9] PROGMEM =
{
  "REMOTE>>",
  "<DIRECT>",
  "TV POWER",
  "HOME",
  "LEFT",
  "UP",
  "RIGHT",
  "DOWN",
  "SELECT",
  "INPUT",
  "VOLUME +",
  "VOLUME -"
};
void remoteLoop2()
{
  // <boilerplate code>
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  IRsend irsend;
  IRaction::setIRsend(irsend);
  Menu* activeMenu;
  // </boilerplate code>

  sonyCodes = new SC;

  byte* add = new byte('+');
  byte* del = new byte('-');
#ifdef USESERIAL
  Serial.println("Big objects created");
#endif
  printDynMem();

  //declare menus
  Menu rootMenu;
  Menu sonyTVmenu(myVFD, 20);
  Menu sonyBRmenu;//TODO
  Menu devices; //TODO
  rootMenu.setVFD(myVFD);

  MenuItem* backItem = new MenuItem(" << BACK", Menupacket::menuOpen, new Menupacket(&activeMenu, &rootMenu, &myScreen)); //no more evil global variables

  //declare menu items
  //rootMenu.add(new MenuItem("Hello", testVoid));

  rootMenu.add(new MenuItem("REMOTE>>", Menupacket::menuOpen, new Menupacket(&activeMenu, &sonyTVmenu, &myScreen)));
  rootMenu.add(new MenuItem("<DIRECT>", bindSonyNav, new sonypacket(&myScreen, &activeMenu, myVFD)));
  rootMenu.add(new MenuItem("FLIP SCR", orient));
  //rootMenu.add(new MenuItem("Devices", Menupacket::menuOpen, new Menupacket(&activeMenu, &devices, &myScreen)); //TODO
  myVFD->setOrientation(1);
  //devices.add(new MenuItem("Sony TV", Menupacket::menuOpen, new Menupacket(&activeMenu, &sonyTVmenu, &myScreen)); //TODO

  //memory management

  //stuff for sony TV
  sonyTVmenu.add(new MenuItem("TV POWER", IRaction::sendCode, sonyCodes->power));
  sonyTVmenu.add(new MenuItem("HOME",  IRaction::sendCode, sonyCodes->home));
  sonyTVmenu.add(new MenuItem("LEFT",  IRaction::sendCode, sonyCodes->left));
  sonyTVmenu.add(new MenuItem("UP",    IRaction::sendCode, sonyCodes->left));
  sonyTVmenu.add(new MenuItem("RIGHT", IRaction::sendCode, sonyCodes->right));
  sonyTVmenu.add(new MenuItem("DOWN",  IRaction::sendCode, sonyCodes->down));
  sonyTVmenu.add(new MenuItem("SELECT", IRaction::sendCode, sonyCodes->select));
  sonyTVmenu.add(new MenuItem("INPUT",   IRaction::sendCode, new IRaction(0xA50, 12)));
  sonyTVmenu.add(new MenuItem("VOLUME +", IRaction::sendCode, new IRaction(0x490, 12)));
  sonyTVmenu.add(new MenuItem("VOLUME -", IRaction::sendCode, new IRaction(0xC90, 12)));
  sonyTVmenu.add(backItem);


  //set starting menu
  activeMenu = &rootMenu;
  MN::bindMN(&myScreen, activeMenu);

  // Serial.println("User data initialized");
  //printDynMem();
  //myVFD->setNum(0,90);
  //myVFD->setNum(1,01);
  myVFD->setMessage(activeMenu->activeItem()->getLabel()); //display the currently highlighted menu item
  char* activemsg=0;
  while (true)
  {
    //    myVFD->setExtra(VFD::_oz, 0);
    myScreen.render(); //handle IR signals

    if (activemsg != activeMenu->activeItem()->getLabel() || myVFD->stateChanged())
    {
      myVFD->setMessage(activeMenu->activeItem()->getLabel()); //display the currently highlighted menu item
      myVFD->stateChanged(); //clear the state checker
    }

    activemsg = (char*)activeMenu->activeItem()->getLabel();

#ifdef DEBUG
    printDynMem(myVFD);
#endif
    if (doRender)
      myVFD->render();

  }
}

