#include "chardefs.h"
//put a message (<=8 characters) for a specified amount of display cycles
void splash(char message[8], unsigned int del)
{
<<<<<<< HEAD
  splashdel = del;
  splashmsg = message;
=======
  splashdel=del;
  splashmsg=message;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
  /*for(int i=0;i<8;i++)
  {
    splashmsg[i]=message[i];
  }*/
}


//return upside down segment pattern given order to follow (trn or tra)
uint16_t transpose(uint16_t in, const byte* pat)
{
<<<<<<< HEAD
  uint16_t result = 0;
  for (int i = 0; i < 16; i++)
  {
    result |= ((in >> 15 - i) & 0x0001) << 15 >> pat[i];
=======
  uint16_t result=0;
  for(int i=0;i<16;i++)
  {
    result |= ((in >> 15-i) & 0x0001) << 15 >> pat[i];
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
  }
  return result;
}

//highspeedKshiftOut
//operate multiple shift registers simultaneously
//using direct memory access for GPIO
void hskshiftOut(byte hsdataPins[], byte data[], byte hsclockPin, byte hslatchPin)
{
<<<<<<< HEAD
  PORTD &= ~ONBIT;
  //disable display while we think

  delayMicroseconds(NPNP_LAG);
  //let the circuit catch up

  PORTB &= ~hslatchPin;
  delayMicroseconds(sdel);
  PORTB |= hslatchPin;
  delayMicroseconds(sdel);
  //reset the shift registers

  const int lim = 4; //number of shift registers
  int p, i; //p is which shift register, i is the bit we're processing

  for (i = 0; i < 8; i++)
  {
    for (p = 0; p < lim; p++)
    {
      if (!!(data[p] & (0x01 << i)))
      {
        PORTB |= hsdataPins[p];
      }
      else
      {
        PORTB &= ~hsdataPins[p];
      }
      //write the shift register data directly through PORTB

    }
    PORTB |= hsclockPin;
    PORTB &= ~hsclockPin;
=======
  PORTD&=~ONBIT; 
  //disable display while we think
  
  delayMicroseconds(NPNP_LAG); 
  //let the circuit catch up
  
  PORTB&=~hslatchPin; 
  delayMicroseconds(sdel);
  PORTB|=hslatchPin;
  delayMicroseconds(sdel);
  //reset the shift registers
  
  const int lim=4; //number of shift registers
  int p,i; //p is which shift register, i is the bit we're processing
  
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
      //write the shift register data directly through PORTB
      
    }
    PORTB|=hsclockPin;
    PORTB&=~hsclockPin;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
    //cycle the clock for the next bit
  }
  //delayMicroseconds(NPNP_LAG); //unnecessary

<<<<<<< HEAD
  PORTD |= ONBIT;
=======
  PORTD|=ONBIT;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
  //re-enable the display
}

//convert number (0-999) to a direct assignable
//code for seven segment displays
uint16_t bts(int num)
{
<<<<<<< HEAD
  uint16_t result = 0x0000;
  result |= (nnums[num / 10]) >> 1 | (nnums[num / 10] & 0x01) << 15;
  result |= nnums[num % 10] << 8;
=======
  uint16_t result=0x0000;
  result|=(nnums[num/10]) >> 1 | (nnums[num/10] & 0x01) << 15;
  result|=nnums[num%10] << 8;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
  return result;
}

uint16_t sshex(byte num)
{
<<<<<<< HEAD
  uint16_t result = 0;
  result |= (nnums[num / 16]) >> 1 | (nnums[num / 16] & 0x01) << 15;
  result |= nnums[num % 16] << 8;
=======
  uint16_t result=0;
  result|=(nnums[num/16]) >> 1 | (nnums[num/16] & 0x01)<<15;
  result|=nnums[num%16]<<8;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
  return result;
}
//AlphaNumericCHaracter
//get a 16 bit variable for direct writing to alphanumeric digits
//for a specified character (in)
uint16_t anch(char in)
{
<<<<<<< HEAD
  switch (in)
  {
    case ' ': return 0;
    case '-': return 1<<14;
    case 0:   return 0;
    case '>': return 0b0000100000001000;
                     //0123456789ABCDEF
      //....
    default:
      if (in >= 'A' && in <= 'z')
      {
        return lcLetters[in - 'A'];
      }
      else if (in >= '0' && in <= '9')
      {
        return anums[in - '0'];
      }
      else
      {
        return 0b0100111000001110;
      }
      break;
=======
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
  else if(in=='-')
  {
    return 1<<14;
  }
  else
  {
    return 0b0100111000001110;
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
  }
}

//flush the display
// (uses data[] as a buffer and hskshiftOut for I/O)
void fdisp(uint16_t* data, byte* dpins)
{
  byte rawData[4];
<<<<<<< HEAD
  for (int i = 0; i < 16; i++)
  {
    //if(data[i]) //slightly brighter but may cause bleeding
    {
      rawData[0] = ~(data[i] & 0xFF);
      rawData[1] = ~((data[i] >> 8) & 0xFF);
      rawData[2] = ~((0x80 >> i) & 0xFF);
      rawData[3] = ~((0x8000 >> i) & 0xFF);
      hskshiftOut(dpins, rawData, CLOCK, RESET);
      delayMicroseconds(HIGHDEL);
=======
  for(int i=0;i<16;i++)
  {
    //if(data[i]) //slightly brighter but may cause bleeding
    {
    rawData[0]=~(data[i]&0xFF);
    rawData[1]=~((data[i]>>8)&0xFF);
    rawData[2]=~((0x80>>i)&0xFF);
    rawData[3]=~((0x8000>>i)&0xFF);
    hskshiftOut(dpins, rawData, CLOCK, RESET);
    delayMicroseconds(HIGHDEL);
>>>>>>> 646047b25fc773f87097bec5cf25867391187927
    }
  }
}
