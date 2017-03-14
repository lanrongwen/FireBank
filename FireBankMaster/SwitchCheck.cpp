#include "Arduino.h"
#include "SwitchCheck.h"

SwitchCheck::SwitchCheck(int Pin){
  _pin = Pin;
   pinMode(_pin, INPUT_PULLUP);
}

bool SwitchCheck::Check(){
  int firstread = 2;
  int secondread = 2;
  do{
    firstread = digitalRead(_pin);      // read input value and store it in val
    delay(10);                         // 10 milliseconds is a good amount of time
    secondread = digitalRead(_pin);     // read the input again to check for bounces
  } while (firstread != secondread);
  if (firstread == HIGH){
    return true;
  }
  return false;
}

