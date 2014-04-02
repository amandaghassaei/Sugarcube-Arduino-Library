//  /*
//    SerialComm.cpp
//    Created by Amanda Ghassaei, Mar 9, 2013.
//  */
  
  #include "SugarCube.h"
  
  SerialComm::SerialComm()
  {
    _sugarcube->setupSerialCommunication();
  }
  
  void SerialComm::buttonStateChanged(byte xPos, byte yPos, boolean state)
  {
    //000xxyys
    byte stateBit = 0;
    if (state) stateBit = 1;
    Serial.write(byte(xPos<<3 | yPos<<1 | stateBit));
  }
  
  void SerialComm::pot1HasChanged(int val)//10 bit
  {
    //001vvvvv - 5 bit val
    Serial.write(byte(1<<5 | (val>>5)));
  }
  
  void SerialComm::pot2HasChanged(int val)//10 bit
  {
    //010vvvvv - 5 bit val
    Serial.write(byte(2<<5 | (val>>5)));
  }
  
  void SerialComm::xAccHasChanged(int val)//0-127
  {
    //011vvvvv - 5 bit val
    Serial.write(byte(3<<5 | (val>>2)));
  }
  
  void SerialComm::yAccHasChanged(int val)//0-127
  {
    //100vvvvv - 5 bit val
    Serial.write(byte(4<<5 | (val>>2)));
  }
