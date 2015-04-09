//help
#ifdef USETIMER
#include <TimerOne.h>
#endif
/*void handleNextCode(unsigned long code, void* vfd)
{
  static int entryNumber=0;
  Serial.println("Handle codes");
  static char activeBtnMsg[8]={0};
  if(activeBtnMsg[0])
  {
    codesOut.write(activeBtnMsg, strlen(activeBtnMsg));
    codesOut.write('|');
    char hexbuffer[9];
    hexbuffer[8]=0;
    for(int i=7;i>=0;i--)
    {
      hexbuffer[i]=((code&0x0F ) < 10 ? '0'+(code&0x0F) : 'A' + (code&0x0F) - 10);
      code>>=4;
    }
    codesOut.write(hexbuffer, 8);
    codesOut.write('\n');
    codesOut.flush();
    Serial.print("Handled code ");
    Serial.print(hexbuffer);
    Serial.print(" for ");
    Serial.println(activeBtnMsg);


  if(codesIn.available())
  {
    char c=' ';
    int i=0;
    for(i=0; i<8 && c!='\n' && codesIn.available(); c++)
    {
      c=activeBtnMsg[i]=codesIn.read();
      Serial.print(c);
    }
    if(i<8)
      activeBtnMsg[i]=0;
  }
  else
    Serial.println("Codes file unavailable");
  ((VFD*)vfd)->setMessage(activeBtnMsg);

}*/

#ifdef USESD
void IRrecLoop()
{
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  //IRsend irsend;
  //IRaction::setIRsend(irsend);
  char activemsg[9] = {0};
  //byte nOfMessages = 0;
  byte activeIndex = 0;
  SD.begin(9);
  unsigned int timer = 0;

  File codesIn =  SD.open("codesIn.txt", FILE_READ);
  //myScreen.bindIRHandler(handleNextCode, myVFD);
  //myScreen.bindButton(_POWER, toggleRender, 0);

  while (true)
  {
    int i;
    myScreen.render(); //handle IR signals
    if (millis() - timer > 75)
    {
      if (codesIn.available())
      {
        myVFD->setExtra(VFD::_fan1, 1);
        if (i > 7)
        {
          for (int x = 0; x < 7; x++)
          {
            activemsg[x] = activemsg[x + 1];
          }
          i = 7;
        }
        activemsg[i++] = codesIn.read();

        myVFD->setMessage(activemsg);
        if (!codesIn.available())
        {
          codesIn.close();
          myVFD->setExtra(VFD::_period, 1);
          activemsg[0] = 0;
        }
      }
      else
      {
        activemsg[7] = 0;
        for (int x = 0; x < 7; x++)
        {
          activemsg[x] = activemsg[x + 1];
        }
        myVFD->setMessage(activemsg);
        myVFD->setExtra(VFD::_fan1, 0);
      }
      timer = millis();


    }

    if (doRender)
      myVFD->render();

  }
}
#endif
#ifdef USETIMER


unsigned long frequency = 1;
VFD* ISRVFD;
void setFreq(unsigned long freq)
{
  static bool hasInterrupt=true;
  if(freq)
  {
    Timer1.initialize(500000 / freq);
    if(!hasInterrupt)
      Timer1.attachInterrupt(timerISR);
  }
  else
  {
    Timer1.detachInterrupt();
    hasInterrupt=false;
    if(ISRVFD)
      ISRVFD->setExtra(VFD::_fan1, false);  
    digitalWrite(A2, false);
  }
}
void incFreq(void*)
{
  frequency++;
  setFreq(frequency);
}
void decFreq(void*)
{
  frequency--;
  setFreq(frequency);
}
void incFreqScale(void*)
{
  frequency *= 10;
  setFreq(frequency);
}
void decFreqScale(void*)
{
  frequency /= 10;
  setFreq(frequency);
}
void pulse(void*)
{
  digitalWrite(A2, HIGH);
  delay(50);
  digitalWrite(A2, LOW);
}
void timerISR()
{
  digitalWrite(A2, !digitalRead(A2));
  if (ISRVFD)
    ISRVFD->setExtra(VFD::_fan1, digitalRead(A2));

}
void fGenLoop()
{
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  ISRVFD = myVFD;
  unsigned long timer = 0;
  myScreen.bindButton(_UP,    incFreq, 0);
  myScreen.bindButton(_DOWN,  decFreq, 0);
  myScreen.bindButton(_LEFT,  incFreqScale, 0);
  myScreen.bindButton(_RIGHT, decFreqScale, 0);
  myScreen.bindButton(_POWER, toggleRender, 0);
  myScreen.bindButton(_SELECT,pulse,        0);
  myScreen.bindIRCode(0xA90,  pulse,        0);

  pinMode(A2, OUTPUT);
  myVFD->setChar(6, 'H');
  myVFD->setChar(7, 'Z');
  setFreq(frequency);
  Timer1.attachInterrupt(timerISR);
  while (true)
  {
    myScreen.render();
    myVFD->setNNum(0, 6, frequency);
    //digitalWrite(A2,LOW);
    myScreen.render();
    myVFD->setNNum(0, 6, frequency);


    if (doRender)
      myVFD->render();
    //myVFD->setExtra(VFD::_fan1, 0);

  }
}
#endif

void analogReaderLoop()
{
  VFD* myVFD = new VFD;
  while(true)
  {
    myVFD->setNNum(0, 4, analogRead(A2));
    myVFD->setNNum(4, 4, analogRead(A0));
    myVFD->setNNum(8, 2, analogRead(A4));
    myVFD->render();
  }
}
void splashLoop(char* msg)
{
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  myVFD->setMessage(msg);
  doRender=true;
  myScreen.bindButton(_POWER, toggleRender, 0);
  while(true)
  {
    myScreen.render();
    if(doRender)
      myVFD->render();
  }
}

void ser2dispLoop()
{
  VFD* myVFD = new VFD;
  Screen myScreen(myVFD);
  //myVFD->setMessage(msg);
  char msg[8];
  myVFD->reset();
  doRender=true;
  myScreen.bindButton(_POWER, toggleRender, 0);
  while(true)
  {
    if(Serial.available())
    {
      for(int i=0; i<8; i++)
      {
        msg[i]=(Serial.available())?Serial.read():0;
      }
      Serial.flush();
      Serial.println(msg);
      myVFD->setMessage(msg);
    }
  
    
    myScreen.render();
    if(doRender)
      myVFD->render();
  }
}
      
