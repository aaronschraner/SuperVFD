#ifndef menu_H
#define menu_H
/*
//#include "MenuItem.h"
class Menu
{
  private:
    int menuPos = 0;
    int maxEntries = 10;
    int totalEntries = 0;
    MenuItem* myItems[10];
    VFD* vfd;
  public:
    
    Menu(VFD* vfd, int maxEntries)
    {
      this->vfd = vfd;
    }
    Menu(VFD* vfd) {
      this->vfd = vfd;
    }

    Menu() {};
    void setVFD(VFD* vfd) {
      this->vfd = vfd;
    }
    void goUp()
    {
      menuPos = (menuPos + totalEntries - 1) % totalEntries;
    }
    void goDown()
    {
      menuPos = (menuPos + 1) % totalEntries;
    }
    void select()
    {
      myItems[menuPos]->execute(vfd);
    }

    void add(MenuItem* entry)
    {
      if (totalEntries < maxEntries)
        myItems[totalEntries++] = entry;
    }

    void toStr(char* str)
    {
      myItems[menuPos]->toStr(str);
    }
    MenuItem* activeItem() {
      return myItems[menuPos];
    }
};*/

#endif
