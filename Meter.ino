#include "Meter.h"
Meter::MeterType Meter::getType()
{
  return type;
}
<<<<<<< HEAD
long long* Meter::getPtr()
=======
long* Meter::getPtr()
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
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
<<<<<<< HEAD
long long Meter::getMeasurement()
{
  return *measure;
}
Meter::Meter(long long* measure)
=======
long Meter::getMeasurement()
{
  return *measure;
}
Meter::Meter(long* measure)
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
{
  this->measure=measure;
  this->pos=pos;
  this->type=Meter::_int2;
}
<<<<<<< HEAD
Meter::Meter(long long* measure, int pos)
=======
Meter::Meter(long* measure, int pos)
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
{
  this->measure=measure;
  this->pos=pos;
  this->type=Meter::_int1;
  
}
<<<<<<< HEAD
Meter::Meter(long long* measure, int pos, Meter::MeterType type)
=======
Meter::Meter(long* measure, int pos, Meter::MeterType type)
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
{
  this->measure=measure;
  this->pos=pos;
  this->type=type;
<<<<<<< HEAD
  if(type ==_intN)
    this->nDigits=8-pos;
}
Meter::Meter(long long* measure, int pos, int nDigits)
=======
}
Meter::Meter(long* measure, int pos, int nDigits)
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
{
  this->type=Meter::_intN;
  this->pos=pos;
  this->measure=measure;
  this->nDigits=nDigits;
}
