class Menu: public MenuEntry
{
  private:
    MenuEntry* entries[10] = {0};
    int entryCount = 0;
    
  public:
    int getEntryCount() {
      return entryCount;
  }
  MenuEntry* getEntry(int pos) {
    return entries[pos];
  }
  
  Menu::MenuEntry(char* label, void(*action)(VFD*), VFD* vfd)
  {
    this->label=label; this->action=action; this->vfd=vfd;
  }
  void add(MenuEntry *entry) {
    entries[entryCount++] = entry;
  }
};
