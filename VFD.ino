
#include "VFD.h"

VFD::VFD(void)
{
  for (int i = 0; i < 10; i++)
  {
    data[i] = 0x0000;
  }
}


void VFD::setOrientation(bool isUpsideDown)
{
  usd = isUpsideDown;
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
}

void VFD::setRaw(int dataReg, uint16_t mask) //strongly discouraged
{
  data[dataReg] = mask;
}

void VFD::unsetRaw(int dataReg, uint16_t mask)//'
{
  data[dataReg] &= ~mask;
}

void VFD::reset()
{
  for (int i = 0; i < 10; i++)
    setRaw(i, 0);
}

void VFD::serialDump()
{
  for (int x = 0; x < 16; x++)
  {
    for (int y = 0; y < 16; y++)
    {
      Serial.print((data[x] >> (15 - y) & 0x01) ? '1' : '0');
    }
    Serial.print("\n");
  }
}

void VFD::setNum(int pos, int num)
{
  setRaw(pos + 8, bts(num % 100));
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
