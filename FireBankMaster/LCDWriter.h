#ifndef LCDWriter_h
#define LCDWriter_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class LCDWriter{
  public:
    LCDWriter();
    void Connect(int LCDReadPin, int LCDWritePin);
    void Display(String FirstLine, String SecondLine , boolean ClearScreen, String Color);
  private:
    SoftwareSerial* _lcd;
    String _prevfirst;
    String _prevsec;
};

#endif

