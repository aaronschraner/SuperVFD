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
  Serial.println("A");
}

void myIRhandler(unsigned long code, void* param)
{
  if (code != ~0)
  {
    Serial.println(code, HEX);
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
  Serial.println("HAXED");
  fanState = !fanState;
  ((VFD*)vfd)->setExtra(VFD::_fan1, fanState);
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
void remoteLoop()
{
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  myParamBlock* mb = new myParamBlock("Ayy", 1, myVFD);
  int myInt;
  
  myScreen.bindIRCode(0xA90, toggleFan, myVFD);
  myScreen.bindIRHandler(myIRhandler, mb);
  
  bool doRender = 1;


  while (true)
  {
    myScreen.render();


    myVFD->setNum(0, myInt);
    if (doRender)
      myVFD->render();
  }
}


