//  /*
//    Boiing.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */

  #include "SugarCube.h"
  
  Boiing::Boiing()
  {
    this->clearAllStorage();
    byte notes[] = {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74};
    for (byte i=0;i<16;i++){
      _notes[i] = notes[i];
    }
  }
  
  void Boiing::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
    }
  }
  
  byte Boiing::absolutePosition(byte pos)
  {
    return (pos+_xOffset)&15;
  }
  
  byte Boiing::relativePosition(byte pos)
  {
    return (pos+16-_xOffset)&15;
  }
  
  void Boiing::wasShaken()
  {
    this->clearAllStorage();
    _sugarcube->clearLEDs();
  }

