#include "Wireless.h"
#include "Charverter.h"


//Constants and Vars//////////////////////////////////////////////////////////
//wireless
//Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used for RX: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69). 
//Not all pins on the  Micro support change interrupts, so only the following can be used for RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI). 
const int WirelessRX = 10;
const int WirelessTX = 11;

//firepins
const int ACTIVATETESTPIN = 2;
const int READTESTPIN = 3;
const int ARMFIRE = 4;
int portpin[15] = { 5, 6, 7, 8, 9, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };

//Testing
const int PORTTESTDURATION = 350;

//TheShow
unsigned long theshow[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long starttime = 0;
unsigned long shottime = -1;
unsigned long currenttime = 0;
//const unsigned long SHOTWINDOW = 250;
const unsigned long SHOTDURATION = 250;

//General
const char NODEID = 'I';
String Command;


//Setup///////////////////////////////////////////////////////////////////////
Wireless mywireless = Wireless();
Charverter charvert = Charverter();

void setup() {
  if(!Serial.available()){
    Serial.begin(9600);
  }
  
  Serial.println("Starting Wireless.");
  mywireless.Connect(WirelessRX,WirelessTX);
  Serial.println("Wireless Started.");

  Serial.println("Setting up PINS.");
  pinMode(ARMFIRE, OUTPUT);
  digitalWrite(ARMFIRE, HIGH);
  
  pinMode(ACTIVATETESTPIN, OUTPUT);
  digitalWrite(ACTIVATETESTPIN, LOW);
  
  pinMode(READTESTPIN, INPUT);
  
  for (int i = 0; i < 15; i++){
    pinMode(portpin[i], OUTPUT);
    digitalWrite(portpin[i], HIGH);
  }
  Serial.println("Pins Set.");

}

//Functions////////////////////////////////////////////////////////////////////
void SyncShow(){
  for (int i = 0; i < 15; i++){
    theshow[i] = 0;
  }
  charvert.ResetString();
  int port = -1;
  unsigned long timemark = 0;
  for(int i = 0; i < Command.length(); i++){
    if (charvert.IsInt(Command[i]) == true){
      charvert.AddChar(Command[i]);
    }
    else if (Command[i] == ','){
      port = charvert.GetInt();
      port--;
      Serial.println(port);
    }
    else if (Command[i] == ';'){
      timemark = charvert.GetLong();
      theshow[port] = timemark;
      Serial.println(timemark);
      Serial.println("Adding");
    }
  }
  for (int i = 0; i < 15; i++){
    if (theshow[i] != 0){
      Serial.print(i);
      Serial.print("  ");
      Serial.println(theshow[i]);
    }
  }
}

void TestShow(){
  unsigned long teststart = 0;
  unsigned long duration = 0;
  int teststatus = LOW;
  
  Serial.println("Resetting for Test.");
  charvert.ResetString();
  for (int i = 0; i < 15; i++){
    digitalWrite(portpin[i], HIGH);
  }
  digitalWrite(ARMFIRE, HIGH);

  for (int i = 0; i < 15; i++){
    if (theshow[i] != 0){
      Serial.print("Starting a Port Test: ");
      Serial.println(i);
      digitalWrite(ACTIVATETESTPIN, HIGH);
      digitalWrite(portpin[i], LOW);
      teststart = millis();
      do{
        teststatus = digitalRead(READTESTPIN);
        duration = millis() - teststart;
        if (teststatus == HIGH){
          Serial.println("Test Passed.");
        }
        else{
          if (duration >= PORTTESTDURATION){
            Serial.println("Test Failed.");
            charvert.AddInt(i + 1);
            charvert.AddChar(' ');
            teststart = 0;
          }
        }
      } while (teststatus == LOW && duration < PORTTESTDURATION);

      digitalWrite(ACTIVATETESTPIN, LOW);
      digitalWrite(portpin[i], HIGH);
    }
  }
  Serial.print("All test complete: ");
  Serial.println(charvert.GetString());
  mywireless.TestResponse(NODEID, charvert.GetString());
}

void StartShow(){
  if (starttime == 0){  
    Serial.println("Resetting for Show.");
    for (int i = 0; i < 15; i++){
      digitalWrite(portpin[i], HIGH);
    }
    digitalWrite(ARMFIRE, HIGH);
    starttime = millis() - 100;
    Serial.print("Starting Show at: ");
    Serial.println(starttime);
  }
  
  for (int i = 0; i < 15; i++){
    if (theshow[i] != 0){
      currenttime = millis();
      shottime = theshow[i] + starttime;
      Serial.print("Port: ");
      Serial.println(i);
      Serial.print("Current Time: ");
      Serial.println(currenttime);
      Serial.print("Shot Time: ");
      Serial.println(shottime);
      Serial.print("Raw Time: ");
      Serial.println(theshow[i]);
      
      if (shottime < currenttime){
        Serial.print("Firing Port: ");
        Serial.println(i);
        
        digitalWrite(portpin[i], LOW);
        digitalWrite(ARMFIRE, LOW);
        
        delay(SHOTDURATION);
        
        digitalWrite(ARMFIRE, HIGH);
        digitalWrite(portpin[i], HIGH);
        theshow[i] = 0;
      }
    }
  }
}

//Main Flow////////////////////////////////////////////////////////////
void loop() {
  Command = mywireless.Receive();
  Serial.print(Command);

  if (Command.startsWith("XXX")){
    Serial.print("Stop Everything\n");
    starttime = 0;
  }

  if (Command.startsWith("SYNC") && Command[4] == NODEID){
    Serial.print("Sync Show\n");
    SyncShow();
  }
  
  if (Command.startsWith("TEST") && Command[4] == NODEID){
    Serial.print("Test Show\n");
    TestShow();
  }

  if (Command.startsWith("START")){
    Serial.print("Start Show\n");
    StartShow();
  }
}
