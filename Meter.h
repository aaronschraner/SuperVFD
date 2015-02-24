#ifndef METER_H
#define METER_H
class Meter
{
  public:
    typedef enum MeterType {_int1, _int2, _int4, _intN};
  
    Meter(long* measure);
    Meter(long* measure, int pos);
    Meter(long* measure, int pos, MeterType type);
    Meter(long* measure, int pos, int nDigits);
    MeterType getType();
    long* getPtr();
    long getMeasurement();
    int getDigits();
    int getpos();
  private:
    int pos;
    MeterType type;
    long* measure;
    int nDigits;
  
};
#endif
