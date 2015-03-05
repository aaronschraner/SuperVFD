#ifndef METER_H
#define METER_H
class Meter
{
  public:
    typedef enum MeterType {_int1, _int2, _int4, _intN};
  
<<<<<<< HEAD
    Meter(long long* measure);
    Meter(long long* measure, int pos);
    Meter(long long* measure, int pos, MeterType type);
    Meter(long long* measure, int pos, int nDigits);
    MeterType getType();
    long long* getPtr();
    long long getMeasurement();
=======
    Meter(long* measure);
    Meter(long* measure, int pos);
    Meter(long* measure, int pos, MeterType type);
    Meter(long* measure, int pos, int nDigits);
    MeterType getType();
    long* getPtr();
    long getMeasurement();
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
    int getDigits();
    int getpos();
  private:
    int pos;
    MeterType type;
<<<<<<< HEAD
    long long* measure;
=======
    long* measure;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
    int nDigits;
  
};
#endif
