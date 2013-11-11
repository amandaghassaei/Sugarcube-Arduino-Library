//  /*
//    SimpleMIDIKeyboard.cpp
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */
  
  #include "FirstPressListener.h"
  #include "SugarCube.h"
  
  void SimpleMIDIKeyboard::buttonPressed(byte xPos, byte yPos)
  {
    _sugarcube->turnOnLED(xPos,yPos);
  }
  
  void SimpleMIDIKeyboard::buttonReleased(byte xPos, byte yPos)
  {
    _sugarcube->turnOffLED(xPos,yPos);
  }
