#ifndef MenuItem_H
#define MenuItem_H
#include "VFD.h"

#include <string.h>
class MenuItem
{
  private:
    enum Type { _noparam, _param };
    char* label;
    Type type;
    void *exec=0;
    void *extra=0;
    //Menu* menuTarget=0;
    //Menu** owningMenu=0;
  public:
    //void (*execute)(VFD* vfd)=0;
    void execute(VFD* vfd)
    {
      if(type==_noparam)
      {
        ((void (*)(VFD*))exec)(vfd);
      }
      else
      {
        ((void (*)(VFD*, void*))exec)(vfd, extra);
      }
    }
    
    MenuItem(char* label, void(*action)(VFD* vfd))
    {
      this->exec = (void*)action;
      this->label = label;
      this->type=_noparam;
    }
    MenuItem(char* label, void(*action)(VFD* vfd, void* packet), void* packet)
    {
      this->exec = (void*)action;
      this->label = label;
      this->type=_param;
      this->extra=packet;
    }
    /*MenuItem(char* label, Menu* toMenu, Menu* &containingMenu)
    {
      menuTarget=toMenu;
      owningMenu=&containingMenu;
      this->label = label;
      this->execute=selectMenu;
    }*/
    void toStr(char* str)
    {
      strncpy(str, label, 8);
    }

    char* getLabel()
    {
      return label;
    }

};/*
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
