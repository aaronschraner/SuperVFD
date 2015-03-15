class SC
{
  public:
    IRaction *up, *down, *right, *left, *select, *power, *home;
    SC()
    {
      //debug("SC constructor");
      up     = new IRaction(0x2F0, 12);
      down   = new IRaction(0xAF0, 12);
      right  = new IRaction(0xCD0, 12);
      left   = new IRaction(0x2D0, 12);
      select = new IRaction(0xA70, 12);
      power  = new IRaction(0xA90, 12);
      home   = new IRaction(0x070, 8);
      printDynMem();
    }
};
