#ifndef METER_H
#define METER_H
class Meter
{
  public:
    typedef enum MeterType {_int1, _int2, _int4, _intN};
  
    Meter(long long* measure);
    Meter(long long* measure, int pos);
    Meter(long long* measure, int pos, MeterType type);
    Meter(long long* measure, int pos, int nDigits);
    MeterType getType();
    long long* getPtr();
    long long getMeasurement();
    int getDigits();
    int getpos();
  private:
    int pos;
    MeterType type;
    long long* measure;
    int nDigits;
  
};
#endif
