#include "Menu.h"

class MenuViewer
{
  private:
    Menu* menu;
    int entryCount;
    int selectedPos;
  
  public:
    MenuViewer(Menu* menu);
    void select();
    void goUp();
    void goDown();
    void toStr(char* str);
};
