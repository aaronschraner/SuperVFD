#include "Meter.h"
Meter::MeterType Meter::getType()
{
  return type;
}
long long* Meter::getPtr()
{
  return measure;
}

int Meter::getDigits()
{
  return nDigits;
}
int Meter::getpos()
{
  return pos;
}
long long Meter::getMeasurement()
{
  return *measure;
}
Meter::Meter(long long* measure)
{
  this->measure=measure;
  this->pos=pos;
  this->type=Meter::_int2;
}
Meter::Meter(long long* measure, int pos)
{
  this->measure=measure;
  this->pos=pos;
  this->type=Meter::_int1;
  
}
Meter::Meter(long long* measure, int pos, Meter::MeterType type)
{
  this->measure=measure;
  this->pos=pos;
  this->type=type;
  if(type ==_intN)
    this->nDigits=8-pos;
}
Meter::Meter(long long* measure, int pos, int nDigits)
{
  this->type=Meter::_intN;
  this->pos=pos;
  this->measure=measure;
  this->nDigits=nDigits;
}

