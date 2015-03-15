class mc
{
  public:


    
    static void hd1(VFD*)     {
      HIGHDEL = 1;
    }
    static void hd10(VFD*)    {
      HIGHDEL = 10;
    }
    static void hd100(VFD*)   {
      HIGHDEL = 100;
    }
    static void hd1000(VFD*)  {
      HIGHDEL = 1000;
    }
    static void hd10000(VFD*) {
      HIGHDEL = 10000;
    }


    static void setPow(VFD* vfd)
    {
      static bool powstate = 0;
      powstate = !powstate;
      vfd->setExtra(VFD::_power, powstate);
    }

    static void toggleLamp(VFD* vfd)
    {
      noInterrupts();
      static bool lampState = 0;
      lampState = !lampState;
      vfd->setExtra(VFD::_fan2, lampState);
      if (lampState)
        for (int i = 0; i < 10; i++)
          send_strip(0x00FFFFFF);
      else
        for (int i = 0; i < 10; i++)
          send_strip(0);
      interrupts();
    }

    static void setFan(VFD* vfd)
    {
      static bool powstate = 0;
      powstate = !powstate;
      vfd->setExtra(VFD::_fan1, powstate);
    }

    static void baspingo(VFD* vfd)
    {
      static bool powstate = 0;
      powstate = !powstate;

      for (int i = 0; i < 10; i++)
      {
        vfd->setRaw(i, (powstate ? 0x00C0 | ((i < 8) ? 0x20 : 0) : 0));
      }

    }

    static void orient(VFD* vfd)
    {
      static bool usd = true;
      vfd->setOrientation(usd);
      usd = !usd;
      vfd->reset();
    }

    static void runStrip(VFD* vfd)
    {
      const int nCols = 11;
      static int colN = 0;
      static unsigned long colors[nCols] = {0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000};
      const unsigned long rgb[3] = {0x00FF0000, 0x000000FF, 0x0000FF00};
      static long long col = -1;
      col = (col + 1) % (nCols);
      for (int i = 0; i < nCols; i++)
        colors[i] = 0;
      if (col < nCols - 1)
        colors[col] = rgb[col % 3];

      noInterrupts();
      for (int i = 0; i < nCols; i++)
        send_strip(colors[i]);
      interrupts();

    }
};

