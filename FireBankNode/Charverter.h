#ifndef Charverter_h
#define Charverter_h

#include "Arduino.h"

class Charverter{
  public:
    Charverter();
    void AddChar(char NewChar);
    void AddString(String NewString);
    void AddInt(int IntToAdd);
    String GetString();
    String ChopString(int ToCut);
    int GetInt();
    unsigned long GetLong();
    bool IsInt(char TestChar);
    bool EndsWith(String TestString);
    bool StartsWith(String TestString);
    bool Contains(char TestChar);
    void ResetString();
  private:
    String _currentstring;
    unsigned long _currentlong;
};

#endif

