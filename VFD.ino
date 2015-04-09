
#include "VFD.h"

VFD::VFD(void)
{
  for (int i = 0; i < 10; i++)
  {
    data[i] = 0x0000;
  }
  schanged = 1;
}


void VFD::setOrientation(bool isUpsideDown)
{
  usd = isUpsideDown;
  uint16_t tmp[10];
  for(int i=0;i<8;i++)
  {
    tmp[i]=transpose(data[7-i],tra);
  }
  tmp[8]=transpose(data[9],trn);
  tmp[9]=transpose(data[8],trn);
  for(int i=0;i<10;i++)
    data[i]=tmp[i];
  schanged = 1;
}

void VFD::setExtra(ExtraLed led, bool state)
{
  byte reg, bits;
  switch (led)
  {
    case _power:  reg = 0; bits = 2; break;

    case _fan0:   reg = 1; bits = 2; break;
    case _fan1:   reg = 1; bits = 4; break;
    case _fan2:   reg = 1; bits = 8; break;

    case _am:     reg = 2; bits = 2; break;
    case _pm:     reg = 2; bits = 4; break;
    case _hand:   reg = 2; bits = 8; break;

    case _oz:     reg = 7; bits = 2; break;
    case _kg:     reg = 7; bits = 4; break;
    case _lbs:    reg = 7; bits = 8; break;

    case _colon:  reg = 9; bits = 4; break;
    case _period: reg = 9; bits = 8; break;
  }
  if (state)
    data[reg] |= bits << 4;
  else
    data[reg] &= ~(bits << 4);
  schanged = 1;
}


void VFD::render()
{
  //updateMeters();
  fdisp(data, dpins);
}

void VFD::setChar(int pos, char ch)
{
  data[(usd ? 7 - pos : pos)] = (data[(usd ? 7 - pos : pos)] & 0x00E0) | (usd ?
                                anch(ch) : //use normal value if right side up
                                transpose(anch(ch), tra)) ; //otherwise flip it
  schanged = 1;
}

void VFD::setRaw(int dataReg, uint16_t mask) //strongly discouraged
{
  data[dataReg] = mask;
  schanged = 1;
}

void VFD::unsetRaw(int dataReg, uint16_t mask)//'
{
  data[dataReg] &= ~mask;
  schanged = 1;
}

void VFD::reset()
{
  for (int i = 0; i < 10; i++)
    setRaw(i, 0);
  schanged = 1;
}

void VFD::serialDump()
{
#ifdef USESERIAL
  for (int x = 0; x < 16; x++)
  {
    for (int y = 0; y < 16; y++)
    {
      Serial.print((data[x] >> (15 - y) & 0x01) ? '1' : '0');
    }
    Serial.print("\n");
  }
#endif
}

void VFD::setNum(int pos, int num)
{
  put(pos + 8, bts(num % 100), 0xFFFF);
}

void VFD::setNNum(int pos, int N,  unsigned long num)
{
  for(int i=0; i<N; i++)
  {
    if(pos+i<8)
      this->setChar(pos+i, '0' + (long)(num / pow(10, N-i-1)) % 10);
    else
      this->setNum(pos+i-8, (int)(num / pow(100, N-2+i)) % 100);
  }
}

void VFD::setMessage(char* message)
{
  int i;
  for (i = 0; i < 8; i++)
    if (message[i])
      this->setChar(i, message[i]);
    else
      break;
  for (i; i < 8; i++)
    this->setChar(i, ' ');
}
void VFD::setMessage(const char* message)
{
  int i;
  for (i = 0; i < 8; i++)
    if (message[i])
      this->setChar(i, message[i]);
    else
      break;
  for (i; i < 8; i++)
    this->setChar(i, ' ');

}
void VFD::put(int pos, uint16_t rawdata)
{
  put(pos, rawdata, 0xFFFF);
}
void VFD::put(int pos, uint16_t rawdata, uint16_t modmask)
{
  if (usd)
  {
    if (pos < 8)
    {
      pos = 7 - pos;
      rawdata = transpose(rawdata, tra);
    }
    else
    {
      pos = 17 - pos;

    }
  }
  else if (pos >= 8)
  {
    rawdata = transpose(rawdata, trn);
  }
  data[pos] = (data[pos] & ~modmask) | (rawdata & modmask);
  schanged = 1;
}
void VFD::setHex(int pos, byte num)
{
  if (pos < 7)
  {
    this->setChar(pos + 1, ((0x0F & num)      < 10) ? '0' +  (num & 0x0F)       : 'A' - 10 +  (num & 0x0F)      );
    this->setChar(pos    , ((0xF0 & num) >> 4 < 10) ? '0' + ((num & 0xF0) >> 4) : 'A' - 10 + ((num & 0x0F) >> 4));
  }
  else if (pos >= 8 && pos < 10)
  {
    this->put(pos, sshex(num));
  }
  schanged = 1;
}
bool VFD::stateChanged()
{
  bool statecpy = schanged;
  schanged = 0;
  return statecpy;
}
bool VFD::getOrientation() const
{
  return usd;
}
