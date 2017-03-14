#ifndef SwitchCheck_h
#define SwitchCheck_h

#include "Arduino.h"

class SwitchCheck{
  public:
    SwitchCheck(int Pin);
    bool Check();
  private:
    int _pin;
};

#endif

