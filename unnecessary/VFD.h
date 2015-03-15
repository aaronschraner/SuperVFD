#ifndef VFD_H
#define VFD_H


#define CLOCK 0x20 //pin 13
#define RESET 0x10 //pin 12
#define DATA0 0x08 //pin 11
#define DATA1 0x04 //pin 10
#define DATA2 0x02 //pin 9
#define DATA3 0x01 //pin 8
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
    byte rawData[4]; //for faster writing to shift registers
    int* toWatch;
//    Meter* meterList[10] = {0};
    
    //byte extraMask[8] = {0};

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
    void setExtra(ExtraLed led, bool state);       
    void render();
    void setChar(int pos, char ch);
    void setRaw(int dataReg, uint16_t mask);
    void unsetRaw(int dataReg, uint16_t mask);
    void serialDump();
//    int  add(Meter* meter);
//    void renderMeter(Meter* meter);
//    void updateMeters();
    void setOrientation(bool isUpsideDown);
//    void del(Meter* meter);
    void reset();
    void setNum(int pos, int num);
};


class Message
{
  private:
    char* contents;//[8] = {0};

  public:
    Message(char* str);
    void truncate();
    void apply(VFD* vfd);
    void set(char* message);

};


#endif

