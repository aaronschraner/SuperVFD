#ifndef VFD_H
#define VFD_H

//#define USEFALLBACK

#ifdef MEGA

#define SDREG PORTB //Shift Data register
#define CRREG PORTB //Clock/Reset register
#define OBREG PORTD //Onbit register

#define CLOCK 0x20 //(CRREG) pin 13
#define RESET 0x10 //(CRREG) pin 12

#define DATA0 0x08 //(SDREG) pin 11
#define DATA1 0x04 //(SDREG) pin 10
#define DATA2 0x02 //(SDREG) pin 9
#define DATA3 0x01 //(SDREG) pin 8

#define ONBIT 0x80 //(OBREG) pin 7

#elif defined(UNO)

#define SDREG PORTD //Shift Data register
#define CRREG PORTB //Clock/Reset register
#define OBREG PORTD //Onbit register

#define CLOCK 0x04 //(CRREG) pin 10
#define RESET 0x01 //(CRREG) pin 8

#define DATA0 0x40 //(SDREG) pin 6
#define DATA1 0x20 //(SDREG) pin 5
#define DATA2 0x10 //(SDREG) pin 4
#define DATA3 0x04 //(SDREG) pin 2

#define ONBIT 0x80 //(OBREG) pin 11

#endif

#define sdel 2
#define NPNP_LAG 2 //Literally so fast we need to worry about transistor lag
//#define HIGHDEL 100 //time to leave display on for fdisp() in microseconds
#ifndef HIGHDEL
long HIGHDEL=1000;
#endif


class VFD
{
  private:
    uint16_t data[10] = {0};//{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    byte dpins[4] = {DATA0, DATA1, DATA2, DATA3};
    int currX = 0;
    bool usd = true;
    bool schanged; //keeps track of state changes
    byte rawData[4]; //for faster writing to shift registers
    
  public:
    enum ExtraLed { _power, //diamond looking thing 
                            _fan0,  //U-shaped part
                            _fan1,  //blade things
                            _fan2,  //bar part
                            _am,    //AM indicator
                            _pm,    //PM indicator
                            _hand,  //hand dial thing
                            _oz,    //Oz indicator
                            _kg,    //kg indicator
                            _lbs,   //Lbs indicator
                            _colon, //colon  (:)
                            _period //period (.)
                          };

    VFD(void);
    void render();
    void setRaw(int dataReg, uint16_t mask);
    void unsetRaw(int dataReg, uint16_t mask);
    void serialDump();
    void setOrientation(bool isUpsideDown);
    void reset();
    
    void setExtra(ExtraLed led, bool state);       
    void setChar(int pos, char ch);
    void setNum(int pos, int num);
    void setNNum(int pos, int N, unsigned long num);
    void setMessage(char* message);
    void setMessage(const char* message);
    void setHex(int pos, byte num);
    
    void put(int pos, uint16_t rawdata);
    void put(int pos, uint16_t rawdata, uint16_t modmask);
    bool stateChanged();
    bool getOrientation() const;
};

#endif

