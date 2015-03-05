
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

void VFD::del(Meter* meter)
{
  for(int i=0;i<10;i++)
  {
    if(meterList[i]==meter)
    {
      meterList[i]=0;
      reset();
      break;
    }
  }
}
void VFD::renderMeter(Meter* meter)
{
  switch (meter->getType())
  {
    case Meter::_int1:
      if (usd)
        data[meter->getpos()] = anums[meter->getMeasurement() % 10];
      else
        data[7 - meter->getpos()] = transpose(anums[meter->getMeasurement() % 10], tra);
      break;
    case Meter::_int2:
      if (usd)
        data[meter->getpos() + 8] = data[meter->getpos() + 8]&0x00C0 | ~0x00C0&bts(meter->getMeasurement() % 100);
      else
        data[(1 - meter->getpos()) + 8] =data[(1 - meter->getpos()) + 8] &0x00C0 | ~0x00C0& transpose(bts(meter->getMeasurement() % 100), trn);
      break;

    case Meter::_int4:
      if (usd)
      {
        data[8] = bts((meter->getMeasurement() / 100) % 100);
        data[9] = data[9] & 0x00C0| (~0x00C0&bts(meter->getMeasurement() % 100));
      }
      else
      {
        data[9] = data[9] & 0x00C0| ~0x00C0&transpose(bts((meter->getMeasurement() / 100) % 100), trn);
        data[8] = transpose(bts(meter->getMeasurement() % 100), trn);
      }
      break;

    case Meter::_intN:
      long meterCopy=meter->getMeasurement();
      if (meterCopy < 0)
      {
        meterCopy=-meterCopy;
      }
      for (int i = 0; i < meter->getDigits(); i++)
      {
        if (i + meter->getpos() < 8)
        {
          if (usd)
          {
            data[meter->getpos()+i] = data[meter->getpos()+i] & 0x00E0 | ~0x00E0&anch((meterCopy / (int)pow(10.0, i * 1.0)) % 10 + '0');
          }
          else
          {
            data[8- i - meter->getpos()] = data[8- i - meter->getpos()] & 0x00E0 | ~0x00E0&transpose(anch((meterCopy / (int)pow(10.0, i * 1.0)) % 10 + '0'), tra);

          }
        }
      }
      /*if(meter->getMeasurement() < 0)
      {
        if (usd)
          data[meter->getpos() - meter->getDigits()] = anch('-');
        else
          data[8 - meter->getDigits() - meter->getpos()] = anch('-');
      }*/
      break;
  }

}


void VFD::updateMeters()
{
  for (int i = 0; (i < 10) && meterList[i]; i++)
  {
    renderMeter(meterList[i]);
  }
}

void VFD::render()
{
  updateMeters();
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
  for(int i=0;i<10;i++)
    setRaw(i,0);
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
int VFD::add(Meter* meter)
{
  int i;
  for (i = 0; meterList[i]; i++);
  if (i < 10)
  {
    meterList[i] = meter;
    return i;
  }
  else
  {
    return -1;
  }
}


/***********************
Stuff for Message class
***********************/



Message::Message(char* str)
{
  contents = str;
}
void Message::truncate()
{
  contents[8] = 0;

}
void Message::apply(VFD* vfd)
{
  bool hbt=true;
  for (byte i = 0;  (i < 8); i++)
  {
    hbt&=!!contents[i];
    vfd->setChar(i, hbt?contents[i]:0);
  }
}

void Message::set(char* message)
{
  contents = message;
}


