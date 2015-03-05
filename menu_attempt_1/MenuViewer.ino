#include "MenuViewer.h"
//#include "Menu.h"

MenuViewer::MenuViewer(Menu* menu)
{
  this->menu=menu;
  this->selectedPos=0;
  this->entryCount=menu->getEntryCount();
}

void MenuViewer::toStr(char* str)
{
  (menu->getEntry(selectedPos))->toStr(str);
}
