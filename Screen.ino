#include "Screen.h"
Screen::Screen(VFD* vfd)
{
  this->vfd = vfd;
}
void Screen::bindButton(remoteBtn btn, void (*action)(void*), void* parameter)
{
  if (btn < _UNDEFINED)
  {
    IRactions[btn] = action;
    IRparams[btn] = parameter;
  }
}
void Screen::bindIRCode(unsigned long code, void (*action)(void*), void* parameter)
{
  for (int i = 0; i < MAX_IR_CODES; i++)
  {
    if (!IRcodes[i])
    {
      IRcodeParams[i] = parameter;
      IRcodeActions[i] = action;
      IRcodes[i] = code;
      break;
    }
  }
}
void Screen::unbindIRcode(unsigned long code)
{
  for (int i = 0; i < MAX_IR_CODES; i++)
  {
    if (IRcodes[i] == code)
    {
      IRcodeParams[i] = 0;
      IRcodeActions[i] = 0;
      IRcodes[i] = 0;
    }
  }
}

void Screen::unbindButton(remoteBtn btn)
{
  //delete IRparams[btn];
  IRactions[btn] = 0;
  IRparams[btn] = 0;
}
void Screen::unbindAllButtons()
{
  for (int i = 0; i < _UNDEFINED; i++)
  {
    unbindButton((remoteBtn)i);
  }
}

void Screen::bindIRHandler(void (*handler) (unsigned long, void*), void* parameter)
{
  allIRHandler = handler;
  allIRparam = parameter;
}

void Screen::unbindIRHandler()
{
  allIRHandler = 0;
  allIRparam = 0;
}

void Screen::render()
{
  bool codeSuccess = false;
  //IR handling//
  if ( irrecv.decode(&results))
  {
    //if (results.value != ~0)
    {
      remoteBtn resp = getBtnByCode(results.value);
      if (IRactions[resp])
      {
        (IRactions[resp])(IRparams[resp]);
      }
      else if (resp = _UNDEFINED)
      {
        for (int i = 0; i < MAX_IR_CODES; i++)
        {
          if (IRcodes[i] == results.value)
          {
            (IRcodeActions[i])(IRcodeParams[i]);
            codeSuccess = true;
          }
        }
      }
      if (allIRHandler && !codeSuccess)
      {
        allIRHandler(results.value, allIRparam);
      }
    }
    irrecv.resume();
  }
}

bool Screen::isBound(remoteBtn btn)
{
  return !!IRactions[btn];
}

/*
void Screen::addRenderFunc(void (*action) (void*), void* parameter)
{
  if(renderFuncCount < MAX_RENDER_FUNCS)
  {
    renderParams[renderFuncCount]=parameter;
    renderFuncs[renderFuncCount++]=action;
  }
}*/
