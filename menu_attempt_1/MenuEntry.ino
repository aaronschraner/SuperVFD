#include "MenuEntry.h"

MenuEntry::MenuEntry(char* label, void(*action)(VFD*), VFD* vfd)
{
  this->action = action;
  this->label=label;
  this->vfd=vfd;
  type=_ACTION;
}

void MenuEntry::toStr(char* str)
{
  str=label;
  if(type==_MENU)
  {
    label[7]='>';
  }
}



void MenuEntry::select()
{
  action(vfd);
}
