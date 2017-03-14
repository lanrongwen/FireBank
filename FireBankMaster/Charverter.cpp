#include "Arduino.h"
#include "Charverter.h"

Charverter::Charverter(){
  _currentstring = "";
  _currentlong = 0;
}

void Charverter::AddChar(char NewChar){
  _currentstring += (char)NewChar;
  if (IsInt(NewChar)){
    _currentlong *= 10;
    _currentlong += ((String)NewChar).toInt();
  }
}

void Charverter::AddString(String NewString){
  _currentstring.concat(NewString);
}

boolean Charverter::IsInt(char TestChar){
  bool isint = true;
  if (TestChar != '0' && TestChar != '1' && TestChar != '2' && TestChar != '3' && TestChar != '4' && TestChar != '5' && TestChar != '6' && TestChar != '7' && TestChar != '8' && TestChar != '9'){
    isint = false;
  }
  return isint;
}

int Charverter::GetInt(){
  int toreturn = _currentstring.toInt();
  _currentstring = "";
  _currentlong = 0;
  return toreturn;
}

unsigned long Charverter::GetLong(){
  unsigned long toreturn = _currentlong;
  _currentstring = "";
  _currentlong = 0;
  return toreturn;
}

void Charverter::AddInt(int IntToAdd){
  _currentstring += String(IntToAdd);
}

void Charverter::ResetString(){
  _currentstring = "";
  _currentlong = 0;
}

String Charverter::GetString(){
  return _currentstring;
}

String Charverter::ChopString(int ToCut){
  int endchar = _currentstring.length() - ToCut;
  return _currentstring.substring(0,endchar);
}

bool Charverter::EndsWith(String TestString){
  return _currentstring.endsWith(TestString);
}

bool Charverter::StartsWith(String TestString){
  return _currentstring.startsWith(TestString);
}

bool Charverter::Contains(char TestChar){
  if (_currentstring.indexOf(TestChar) >= 0){
    return true;
  }
  return false;
}

