
enum remoteBtn {
  _POWER,
  _A,
  _B,
  _C,
  _UP,
  _LEFT,
  _SELECT,
  _RIGHT,
  _DOWN,
  _UNDEFINED
};
const unsigned long btnCodes[_UNDEFINED] = {
  0x10EFD827,
  0x10EFF807,
  0x10EF7887,
  0x10EF58A7,
  0x10EFA05F,
  0x10EF10EF,
  0x10EF20DF,
  0x10EF807F,
  0x10EF00FF
};

remoteBtn getBtnByCode(unsigned long code)

{
  if ((code & 0xFFFF0000) != 0x10EF0000)
    return _UNDEFINED;
  for (int i = _POWER; i <= _DOWN; i++)
  {
    if (code ==  btnCodes[i])
      return (remoteBtn)i;
  }
  return _UNDEFINED;
}


