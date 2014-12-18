//really big VFD driver
#include <EEPROM.h>
#define CLOCK 0x20 //pin 13
#define RESET 0x10 //pin 12
#define DATA0 0x08 //pin 11
#define DATA1 0x04 //pin 10
#define DATA2 0x02 //pin 9
#define DATA3 0x01 //pin 8
#define ONBIT 0x80 //pin 7 (PORTD)
#define sdel 2
#define ADDR_CANS 0x10 //arbitrary

void hskshiftOut(byte hsdataPins[], byte data[], byte hsclockPin, byte hslatchPin)
{
  PORTD&=~ONBIT;
  PORTB&=~hslatchPin;
  delayMicroseconds(sdel);
  PORTB|=hslatchPin;
  delayMicroseconds(sdel);
  //delay(100);
  const int lim=4;
  int p,i;
  for(i=0;i<8;i++)
  {
    for(p=0;p<lim;p++)
    {
      if(!!(data[p] & (0x01<<i)))
      {
        PORTB|=hsdataPins[p];
      }
      else
      {
        PORTB&=~hsdataPins[p];
      }
      //delay(10);
    }
    PORTB|=hsclockPin;
    //delay(100);
    PORTB&=~hsclockPin;
    //delay(100);
  }
  PORTD|=ONBIT;
}
byte dpins[4]={DATA0, DATA1, DATA2, DATA3};
byte rawData[4];
uint16_t data[10] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
const uint16_t lcLetters[] = 
{
  //0123456789X12345
  0b1111000000010001, //A
  0b0110010100010101, //B
  0b1001000100010000, //C
  0b0010010100010101, //D
  0b1101000100010000, //E
  0b1101000000010000, //F
  0b1011000100010000, //G
  0b1111000000000001, //H
  0b0000010100010100, //I
  0b0011000100010001, //J
  0b1111000000000010, //K
  0b1001000100000000, //L
  0b1011000000001011, //M
  0b1011001000001001, //N
  0b1011000100010001, //O
  0b1101000000010001, //P
  0b1011001100010001, //Q
  0b1101001000010001, //R
  0b1110000100010000, //S
  0b0000010000010100, //T
  0b1011000100000001, //U
  0b1010001000000001, //V
  0b1011101000000001, //W
  0b0000101000001010, //X
  0b1110000100000001, //Y
  0b0000100100010010, //Z
  0b1001000100010000, //[
  0b0000001000001000, //\_ //backslashes are weird
  0b0010000100010001, //]
  0b0000000000011010, //^
  0b0000000100000000, //_
  0b0000000000001000, //`
  0b0111000100010001, //a
  0b1111000100000000, //b
  0b0101000100000000, //c
  0b0111000100000001, //d
  0b0101100100000000, //e
  0b0100010000000010, //f
  0b1110100100010001, //g
  0b1111000000000000, //h
  0b0000010000000100, //i
  0b0010100100000001, //j
  0b1001001000000010, //k
  0b1001100000000000, //l
  0b0111010000000000, //m
  0b0111000000000000, //n
  0b0111000100000000, //o
  0b0100010000010101, //p
  0b1100001000010001, //q
  0b0101000000000000, //r
  0b0000001100011000, //s
  0b0100010000000100, //t
  0b0011000100000000, //u
  0b0010001000000000, //v
  0b0011101000000000, //w
  0b0000101000001010, //x
  0b0000010000001010, //y
  0b0100100100000000, //z
};
const uint16_t anums[10]=
{
  //0123456789X12345
  0b1011100100010011, //0
  0b0010000000000001, //1
  0b0101000100010001, //2
  0b0110000100010001, //3
  0b1110000000000001, //4
  0b1110000100010000, //5
  0b1111000100010000, //6
  0b0000010000010010, //7
  0b1111000100010001, //8
  0b1110000100010001, //9
  //0123456789X12345
};
const uint8_t nnums[10] = 
{
  //1234567
  0b1110111, //0
  0b0100100, //1
  0b1101011, //2
  0b1101101, //3
  0b0111100, //4
  0b1011101, //5
  0b1011111, //6
  0b1100100, //7
  0b1111111, //8
  0b1111101, //9
  
};
uint16_t bts(int num)
{
  uint16_t result=0x0000;
  result|=(nnums[num/10]) >> 1 | (nnums[num/10] & 0x01) << 15;
  result|=nnums[num%10] << 8;
  return result;
}
char cmax='z';
uint16_t anch(char in)
{
  if(in >= 'A' && in <= 'z')
  {
    return lcLetters[in-'A'];
  }
  else if(in == ' ')
  {
    return 0;
  }
  else if(in >= '0' && in <= '9')
  {
    return anums[in-'0'];
  }
  else
  {
    return 0b0100111000001110;
  }
}
void fdisp()
{
  byte baketarget[12];
  for(int i=0;i<16;i++)
  {
    rawData[0]=~(data[i]&0xFF);
    rawData[1]=~((data[i]>>8)&0xFF);
    rawData[2]=~((0x80>>i)&0xFF);
    rawData[3]=~((0x8000>>i)&0xFF);
    hskshiftOut(dpins, rawData, CLOCK, RESET);
    //bake(dpins, rawData);
    //bso(ref, RESET, CLOCK);
    //delay(200);
    delayMicroseconds(1000);
  }
}
unsigned short cans=0;
unsigned int splashdel=0;
char splashmsg[8];
void splash(char message[8], unsigned int del)
{
  splashdel=del;
  for(int i=0;i<8;i++)
  {
    splashmsg[i]=message[i];
  }
}
boolean usd=false;
void setup()
{
  
  DDRB|=0x3F; //set directions
  DDRD|=ONBIT;
  for(int i=0;i<10;i++)
    data[i]=0;
    fdisp();
  Serial.begin(9600);
  cans = EEPROM.read(ADDR_CANS)<<8 | EEPROM.read(ADDR_CANS+1);
  
}
int mydel=200;
byte trn[16]=
{
  1, //0
  0, //1
  15,//2
  14,//3
  13,//4
  12,//5
  11,//6
  10,//7
  8, //6
  9, //9
  7, //10
  6, //11
  5, //12
  4, //13
  3, //14
  2  //15
};
byte tra[16]=
{
  2, //0
  1, //1
  0, //2
  15,//3
  14,//4
  13,//5
  12,//6
  11,//7
  8, //8
  9, //9
  10,//10
  7, //11
  6, //12
  5, //13
  4, //14
  3, //15
};
uint16_t transpose(uint16_t in, byte* pat)
{
  uint16_t result=0;
  for(int i=0;i<16;i++)
  {
    result |= ((in >> 15-i) & 0x0001) << 15 >> pat[i];
  }
  return result;
}
unsigned long mymillis;
int backset;
boolean lr6=false, lr5=false;
char msg[]="    CANS";
unsigned int candel=500;
unsigned long canctr=0;
void loop()
{
  msg[0]=(cans/100)%10 + '0';
  msg[1]=(cans/10)%10 + '0';
  msg[2]=cans%10 + '0';
  for(int c=0;c<10;c++)
  {
    if(c<8)
    {
      data[(usd?c:7-c)]=(usd?
          transpose(anch((splashdel>0?splashmsg[c]:msg[c])),tra):
          anch((splashdel>0?splashmsg[c]:msg[c])));
      //data[7-c] |= 0x00E0;
    }
  }
  if(Serial.available())
  {
    if(Serial.peek() == '@')
    {
      Serial.read();
      usd=!usd;
    }
    else
    {
    int i;
    i=0;
    for(int x=0;x<8;x++)
    {
      msg[x]=' ';
    }
    while(Serial.available())
    {
      msg[i]=Serial.read();
      i++;
    }
    }
  }
  if(!digitalRead(6)&&!lr6&&millis() > canctr)
  {
    cans++;
    EEPROM.write(ADDR_CANS,cans>>8 & 0xFF);
    EEPROM.write(ADDR_CANS+1,cans & 0xFF);
    canctr=millis() + candel;
  }
  
  lr6=!digitalRead(6);
  if(!digitalRead(5)&&!lr5)
  {
    cans=0;
    EEPROM.write(ADDR_CANS, 0);
    EEPROM.write(ADDR_CANS+1, 0);
    lr5=true;
    delay(10);
    splash("Reset   ", 20);
  }
  lr5=!digitalRead(5);
  data[0]|=(lr6?0x00E0:0);
  data[9]=(usd?transpose(bts((millis()/1000/60)%60),trn):bts((millis()/1000)%60));
  data[8]=(usd?transpose(bts((millis()/1000)%60),trn):bts((millis()/1000/60)%60));
  //data[2]|=0x0060;// AM/PM indicators
  /*for(int i=0;i<10; i++)
  {
    for(int p=0;p<16;p++)
    {
      //data[i]|=0x8000>>p;*/
      //mymillis=millis();
      //while(millis() < mymillis + mydel)
      //{
        data[9]|=(millis()%1000<500 ? 0x40 : 0);
        fdisp();
        delay(1);
      //}
      //backset = (backset+1)%100;
      /*
    }
  }*/
  if(splashdel>0)
    splashdel--;
}
