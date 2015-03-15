#include <IRremoteInt.h>
void restoreTimer()
{
  TIMER_CONFIG_NORMAL();
  TIMER_ENABLE_INTR;
}

class IRaction
{
  public:
    uint32_t code;
    long freq;
    long len;
    static IRsend irsend;
    IRaction(uint32_t code, long len = 12, long freq = 40)
    {
      this->len = len;
      this->code = code;
      this->freq = freq;
    }
    
    static void setIRsend(IRsend ir)
    {
      irsend=ir;
    }
    void send(IRsend irsend)
    {
      if (freq == 40)
      {
        for (int i = 0; i < 3; i++)
        {
          irsend.sendSony(code, 12);
          delay(40);
        }
      }
      restoreTimer();
    }

    static void sendCode(VFD* vfd, void* iraction)
    {
      ((IRaction*)iraction)->send(irsend);
    }
    static void sendCode(void* iraction)
    {
      ((IRaction*)iraction)->send(irsend);
    }
};
