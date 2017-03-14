#include "Arduino.h"
#include "Wireless.h"
#include <SoftwareSerial.h>
#include "Charverter.h"

Wireless::Wireless() : _wireless(NULL) {
}

void Wireless::Connect(int ReadPin, int WritePin){
   Serial.println("removing old wireless");
  if (NULL != _wireless){
    delete _wireless;
  }
  Serial.println("wireless connecting...");
  _wireless = new SoftwareSerial(ReadPin,WritePin); //rx tx
  _wireless->begin(9600);
  if (_wireless->available()){
    Serial.print("wireless connected");
  }
}

void Wireless::Stop(){
  _wireless->print("XXX\n");
}

void Wireless::SyncShow(int nodeid, String Sequence){
  char node = (nodeid + 65);
  _wireless->print("SYNC");
  _wireless->print(node);
  _wireless->print(Sequence);
  _wireless->print("CNYS\n");
}

String Wireless::Test(int nodeid){
  char node = (nodeid + 65);
  _mystring.ResetString();
  _wireless->print("TEST");
  _wireless->print(node);
  _wireless->print("TSET\n");
  unsigned long teststart = millis();
  unsigned long duration = 0;
  do{
    if (_wireless->available()){
      _mystring.AddChar(_wireless->read());
    }
    duration = millis() - teststart;
  } while (_mystring.EndsWith("\n") == false && duration < 10000);
  if (duration >= 10000){
    return "Unresponsive";
  }
  return _mystring.ChopString(1);
}

void Wireless::TestResponse(char NodeId, String Failures){
  if (Failures.length() > 0){
    _wireless->print(NodeId);
    _wireless->print(Failures);
    _wireless->print("\n");
  }
  else{
    _wireless->print(NodeId);
    _wireless->print("Success\n");
  }
}

void Wireless::Start(){
  _wireless->print("START\n");
}

void Wireless::SendChar(char Message){
  _wireless->print(Message);
}

void Wireless::Send(String Message){
  if (Message != ""){
    _wireless->print(Message);
  }
}

String Wireless::Receive(){
  _mystring.ResetString();
  do{
    if (_wireless->available()){
      _mystring.AddChar(_wireless->read());
    }
  } while (_mystring.EndsWith("\n") == false);
  return _mystring.GetString();
}

