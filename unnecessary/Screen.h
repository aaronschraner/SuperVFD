#ifndef Screen_H
#define Screen_H
#define MAX_RENDER_FUNCS 5
#define MAX_IR_CODES 10
class Screen
{
  private:
    void (*IRactions[9])(void*) = {0};
    void* IRparams[9] = {0};
    void (*allIRHandler)(unsigned long, void*)=0;
    void* allIRparam = 0;
    void (*renderFuncs[MAX_RENDER_FUNCS])(void*) = {0};
    void* renderParams[MAX_RENDER_FUNCS] = {0};
    int renderFuncCount=0;
    void (*IRcodeActions[MAX_IR_CODES])(void*)={0};
    void* IRcodeParams[MAX_IR_CODES]={0};
    unsigned long IRcodes[MAX_IR_CODES]={0};
    
  public:
    VFD* vfd;
    Screen(VFD* vfd);
    void bindButton(remoteBtn btn, void(*action)(void*), void* parameter);
    void unbindButton(remoteBtn btn);
    void unbindAllButtons();
    void bindIRHandler(void (*handler) (unsigned long, void*), void* parameter);
    void unbindIRHandler();
    void render();
    void bindIRCode(unsigned long code, void (*action)(void*), void* parameter);
    void unbindIRcode(unsigned long code);
    
};
#endif
