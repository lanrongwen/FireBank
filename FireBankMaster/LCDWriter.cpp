#include "Arduino.h"
#include "LCDWriter.h"
#include <SoftwareSerial.h>

LCDWriter::LCDWriter() : _lcd(NULL) {
  _prevfirst = "";
  _prevsec = "";
}

void LCDWriter::Connect(int LCDReadPin, int LCDWritePin){
 Serial.println("removing old LCD");
  if (NULL != _lcd){
    delete _lcd;
  }
  Serial.println("LCD connecting...");
  _lcd = new SoftwareSerial(LCDReadPin,LCDWritePin); //rx tx
  _lcd->begin(9600);
  if (_lcd->available()){
    Serial.println("LCD connected");
  }

  // set the size of the display if it isn't 16x2 (you only have to do this once)
  _lcd->write(0xFE);
  _lcd->write(0xD1);
  _lcd->write(16);  // 16 columns
  _lcd->write(2);   // 2 rows
  delay(10);       

  // set the contrast, 200 is a good place to start, adjust as desiRED
  _lcd->write(0xFE);
  _lcd->write(0x50);
  _lcd->write(200);
  delay(10);       

  // set the brightness - we'll max it (255 is max brightness)
  _lcd->write(0xFE);
  _lcd->write(0x99);
  _lcd->write(255);
  delay(10);       

  // turn off cursors
  _lcd->write(0xFE);
  _lcd->write(0x4B);
  _lcd->write(0xFE);
  _lcd->write(0x54);

  // clear screen
  _lcd->write(0xFE);
  _lcd->write(0x58);
  delay(10);   // we suggest putting delays after each command 

  // go 'home'
  _lcd->write(0xFE);
  _lcd->write(0x48);
  delay(10);   // we suggest putting delays after each command 

  Display("Wireless Fire","Setting Up...",true,"RED");
}


void LCDWriter::Display(String FirstLine, String SecondLine , boolean ClearScreen, String Color){
  if ((FirstLine != _prevfirst) || (SecondLine != _prevsec)){
    _prevfirst = FirstLine;
    _prevsec = SecondLine;
      
    _lcd->write(0xFE);
    _lcd->write(0xD0);
    if (Color == "RED"){
      _lcd->write(255); 
      _lcd->write((uint8_t)0);
      _lcd->write((uint8_t)0);
    }
    else if (Color == "GREEN"){
      _lcd->write((uint8_t)0); 
      _lcd->write(255);
      _lcd->write((uint8_t)0);
    }
    else if (Color == "ORANGE"){
      _lcd->write(255); 
      _lcd->write(102);
      _lcd->write((uint8_t)0);
    }
    else if (Color == "RANDOM"){
      _lcd->write(random(1,255)); 
      _lcd->write(random(1,255));
      _lcd->write(random(1,255));
    }
    else{ //BLUE is default
      _lcd->write((uint8_t)0); 
      _lcd->write((uint8_t)0);
      _lcd->write(255);
    }
    
    delay(10);  // give it some time to adjust the backlight!
    if (ClearScreen == true){
      // clear screen
      _lcd->write(0xFE);
      _lcd->write(0x58);
      delay(10);   // we suggest putting delays after each command 
    }
    _lcd->println(FirstLine);
    _lcd->print(SecondLine);
  }
}


