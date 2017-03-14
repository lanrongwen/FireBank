#ifndef Wireless_h
#define Wireless_h

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Charverter.h"

class Wireless{
  public:
    Wireless();
    void Connect(int ReadPin, int WritePin);
    void Stop();
    void SyncShow(int nodeid, String Sequence);
    String Test(int nodeid);
    void TestResponse(char NodeId, String Failures);
    void Start();
    void Send(String Message);
    void SendChar(char Message);
    String Receive();
  private:
    SoftwareSerial* _wireless;
    Charverter _mystring;
};

#endif

