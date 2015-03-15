
/*void spacify(char msg[])
{
  for (int i = 0; i < 9; i++)
  {
    if (msg[i] == 0)
    {
      if (i < 8)
      {
        msg[i] = ' ';
        msg[i + 1] = 0;

      }
    }
  }
}
*/

//put the message in the draw buffer
/*for (int c = 0; c < 10; c++)
{
  if (c < 8)
  {
    data[(usd ? 7 - c : c)] = (usd ?
                               anch((splashdel > 0 ? splashmsg[c] : msg[c])) : //use normal value if right side up
                                 transpose(anch((splashdel > 0 ? splashmsg[c] : msg[c])), tra)); //otherwise flip it
    //data[7-c] |= 0x00E0; //for turning on pretty lights n stuff
  }
}*/
//msg = msgbuffer + backset;
/*if (millis() > backsetctr + 300 && strlen(msgbuffer) > 8)
{
  backset = (backset + 1) % 256;
  backsetctr = millis();
  spacify(msg);
}
int reg;
*/
/*
  //input processing
  if (Serial.available())
  {
    int x=1000;
    switch(Serial.peek())
    {
      case '@':
        Serial.read();
        usd = !usd;
        Serial.println(usd ? "Now upside down" : "Now right side up");
       //toggle orientation when '@' sent via serial
       break;
      case '#':
        timerOn=!timerOn;
        Serial.read();
        delay(2);
        break;
      case '!':
        mymillis=millis();
        Serial.read();
        break;
      case '%':
        Serial.read();
        while(!Serial.available() && x)
        {
          x--;
          delayMicroseconds(100);
        }
        reg=Serial.read()-'0';

        switch(Serial.read())
        {
          case 'p':
            miscmask[0]|=0x0020;
            break;
          case 'P':
            miscmask[0]&=~0x0020;
            break;
          case '-':
            miscmask[1]|=0x0080;
            break;
          case '_':
            miscmask[1]&=~0x0080;
            break;
        }
        break;
      default:
        processSerial();
        msg = msgbuffer; //+ backset;
        spacify(msg);
        backsetctr=millis();
        break;
    }
  }*/
/*for(int i=0;i<8;i++)
{
  data[i]=anch((((int)msg)/((int)pow(10,i)))%10+'0');
}*/
/*for(int i=0;i<8;i++)
{
  data[i]=anch('0'+(0x01&(msg[0]>>i)));
}*/
//data[9] = sshex(msg[0]);
//data[1]=0x0080;
//data[0] |= (lr6 ? 0x00E0 : 0); //flash an indicator if something is triggering the can sensor
/*for(int i=0;i<10;i++)
{
  data[i]|=miscmask[i];
}*/
/*if(timerOn)
{
  data[9] = (usd ? bts(((millis() - mymillis) / 1000) % 60) : transpose(bts(((millis() - mymillis) / 1000 / 60) % 60), trn)); //timer (uses millis(), I know I need an RTC eventually.)
  data[8] = (usd ? bts(((millis() - mymillis) / 1000 / 60) % 60) : transpose(bts(((millis() - mymillis)/ 1000) % 60), trn));
  data[9] |= (millis() % 1000 < 500 ? 0x40 : 0); //add flashing colon to timer
}
else
{
  //data[9]=0;
  data[8]=0;
}
//data[2]|=0x0060;// AM/PM indicators
*/
/*
  fdisp();
  delay(1);
  if (splashdel > 0)
    splashdel--;
    */
    
    


