#ifndef menu_H
#define menu_H

#include "MenuItem.h"

//class for menu navigation
class Menu
{
  private:
    int menuPos = 0;
    const int maxEntries=10;
    int totalEntries = 0;
    MenuItem* myItems[10];
    VFD* vfd;
  public:
    //declare with nonstandard number of item slots
    Menu(VFD* vfd, int me):maxEntries(me)
    {
      this->vfd = vfd;
    }
    
    //declare with default 10 item slots
    Menu(VFD* vfd):maxEntries(10)
    {
      this->vfd = vfd;
    }

    Menu() {};
    
    //mostly deprecated
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
    
    //execute the action associated with the selected menu item
    //TODO: switch to void* with parameter model
    void select()
    {
      myItems[menuPos]->execute(vfd);
    }

    //add a menu item to the menu
    void add(MenuItem* entry)
    {
      if (totalEntries < maxEntries && entry)
        myItems[totalEntries++] = entry;
    }

    //pretty much deprecated
    void toStr(char* str)
    {
      myItems[menuPos]->toStr(str);
    }
    
    //get pointer to active menuitem
    MenuItem* activeItem() {
      return myItems[menuPos];
    }
};

#endif
