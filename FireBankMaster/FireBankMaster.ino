#include "LCDWriter.h"
#include "SwitchCheck.h"
#include "Wireless.h"
#include "Charverter.h"

//sd classes
#include <SPI.h>
#include <SdFat.h>

//Constants and Vars//////////////////////////////////////////////////////////
//Switches
const int TESTSWITCH = 3;
const int STARTSWITCH = 4;
const int STOPALLSWITCH = 2;

//display
const int LCDDisplayOUT = 5;
const int LCDDisplayIN = 9; // not used really.... 

//wireless
//Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used for RX: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69). 
//Not all pins on the  Micro support change interrupts, so only the following can be used for RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI). 
const int WirelessRX = 12;
const int WirelessTX = 11;

//SDCard
const int SDcs = 53;
const int SDsi = 51;
const int SDso = 50;
const int SDclk = 52;
SdFat SD;
File myFile;
bool fileloaded = false;

//TheShow
unsigned long theshow[26][16] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
bool showloaded = false;

//Test
int TestNodeIndex = 0;

//Setup///////////////////////////////////////////////////////////////////////
LCDWriter mylcd = LCDWriter();
Wireless mywireless = Wireless();
SwitchCheck testswitch = SwitchCheck(TESTSWITCH);
SwitchCheck startswitch = SwitchCheck(STARTSWITCH);
SwitchCheck stopallswitch = SwitchCheck(STOPALLSWITCH);
Charverter charvert = Charverter();

void setup() {
  if(!Serial.available()){
    Serial.begin(9600);
  }
  
  Serial.println("Starting LCD.");
  mylcd.Connect(LCDDisplayIN,LCDDisplayOUT);
  mylcd.Display("Starting Up","The Show Master",true,"ORANGE");
  Serial.println("LCD Started.");

  Serial.println("Starting Wireless.");
  mywireless.Connect(WirelessRX,WirelessTX);
  Serial.println("Wireless Started.");

  Serial.println("Starting SD Card.");
  pinMode(53, OUTPUT);
  //pinMode(10, OUTPUT);
  //digitalWrite(10, HIGH);
  digitalWrite(53, HIGH);
  boolean done = false;
  do{
    delay(150);
    if (!SD.begin(SDcs,SPI_QUARTER_SPEED)) {
      mylcd.Display("Failed to load.","SD reader",true,"RED");
      do{
        ;
      } while(true);
    }
    else{
      mylcd.Display("SD Loaded","Ready for fun!",true,"GREEN");
      delay(2000);
      done = true;
    }
  }while(done == false);
  Serial.println("SD Card Started.");

}

//Functions////////////////////////////////////////////////////////////////////
void LoadFile(){
  if (fileloaded == false){
    mylcd.Display("Loading","Show.txt",true,"BLUE");
    delay(1500);
    if (SD.exists("Show.txt")){
      myFile = SD.open("Show.txt");
      fileloaded = true;
      PopulateShow();
    }
    else{
      mylcd.Display("Failed to load"," ",true,"RED");
      fileloaded = false;
      delay(2000);
    }
  }
}

void PopulateShow(){
  char toread;
  unsigned long shottime = 0;
  char node;
  int port;
  int nodeindex;
  do{
    if (myFile.available()){
      toread = myFile.read();
      if (charvert.IsInt(toread)){
        charvert.AddChar(toread);
      }
      else{
        if (shottime == 0){
          shottime = charvert.GetLong();
          node = toread;
        }
        else{
          port = charvert.GetInt();
          if (node >= 96){
            nodeindex = node - 97;
          }
          else{
            nodeindex = node - 65;
          }
          theshow[nodeindex][port] = shottime;
          theshow[nodeindex][0] = 1;
          shottime = 0;
        }
      }
    }
    else{
      showloaded = true;
    }
  } while (showloaded == false);
  Serial.println("Show Loaded.");
}

void SyncShow(){
  for (int i = 0; i < 26; i++){
    if (theshow[i][0] == 1){
      Serial.print("Found entries to send.");
      charvert.ResetString();
      for (int j = 1; j < 16; j++){
        if (theshow[i][j] != 0){
          charvert.AddString(String(j));
          charvert.AddChar(',');
          charvert.AddString(String(theshow[i][j]));
          charvert.AddChar(';');
          Serial.println(charvert.GetString());
        }
      }
      mywireless.SyncShow(i,charvert.GetString());
    }
  }
}

void TestShow(){
  Serial.println(TestNodeIndex);
  
  if (TestNodeIndex >= 26){
    TestNodeIndex = 0;
  }

  if (theshow[TestNodeIndex][0] == 1){
    charvert.ResetString();
    char node = TestNodeIndex + 65;
    Serial.println("Found entries to test.");
    mylcd.Display("Now Testing:", String(node), true, "ORANGE");
    String badnodes = mywireless.Test(TestNodeIndex);
    if (badnodes.endsWith("Success")) {
      mylcd.Display(String(node), "Is Good!", true, "GREEN");
      delay(2000);
    }
    else {
      charvert.AddString("Fail Node: ");
      char node = TestNodeIndex + 65;
      charvert.AddChar(node);
      if (badnodes.length() > 15){
        mylcd.Display(charvert.GetString(), badnodes.substring(0,15), true, "RED");
        delay(15000);
        mylcd.Display(charvert.GetString(), badnodes.substring(16), true, "RED");
      }
      else{
        mylcd.Display(charvert.GetString(), badnodes, true, "RED");
      }
      delay(15000);
    }
  }
  TestNodeIndex++;
}

//Main Flow////////////////////////////////////////////////////////////
void loop() {
  if (stopallswitch.Check() == true){
    mylcd.Display("Stop","Show",true,"RED");
    mywireless.Stop();
    showloaded = false;
  }
  else{
    if (showloaded == false){
      mylcd.Display("Loading and"," Syncing Show",true,"ORANGE");
      LoadFile();
      PopulateShow();
      SyncShow();
    }

    if (testswitch.Check() == true){
     // mylcd.Display("Will Test","",true,"BLUE");
      TestShow();
    }
    else{
     
      if (startswitch.Check() == true){
        mylcd.Display("Enjoy the","SHOW",true,"RANDOM");
        mywireless.Start();
        //can put on display...
        delay(500);
      }
      else{
         mylcd.Display("May","Progress",true,"GREEN");
      }
    }
  }
}
