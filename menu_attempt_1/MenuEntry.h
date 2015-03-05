class MenuEntry
{
public:
    MenuEntry(char* label, void(*action)(VFD*), VFD* vfd);
    //MenuEntry(char* label, Menu* menu);
  
    enum EntryType {
      _MENU,
      _ACTION
    };

    void toStr(char* str);
    void select();

  protected:
    char* label;
    EntryType type;

  
  private:
    void (*action)(VFD*);
    VFD* vfd;

};
/*
class RootMenu:
{
  public:
    void add(Menu* menu);
    void add(MenuEntry* menuEntry);
    int getEntries();

};

class MenuViewer
{
  public:
    MenuViewer(RootMenu* rootMenu);
    void select(MenuEntry* menuEntry);
    void scrollUp();
    void scrollDown();
    void goBack();
    void getLabel(char* label);
    int getEntries();
    void load(Menu* menu);
};*/

