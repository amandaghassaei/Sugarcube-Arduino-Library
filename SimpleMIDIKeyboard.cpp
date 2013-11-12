//  /*
//    SimpleMIDIKeyboard.cpp
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */
  
  #include "SugarCube.h"
  
  SimpleMIDIKeyboard::SimpleMIDIKeyboard()
  {
    _baseNote = 50;
    _velocity = 100;
  }
  
  void SimpleMIDIKeyboard::buttonPressed(byte xPos, byte yPos)
  {
    _sugarcube->turnOnLED(xPos,yPos);
    _sugarcube->noteOn(createMIDINoteInFourths(xPos, yPos, _baseNote), _velocity);
  }
  
  void SimpleMIDIKeyboard::buttonReleased(byte xPos, byte yPos)
  {
    _sugarcube->turnOffLED(xPos,yPos);
    _sugarcube->noteOff(createMIDINoteInFourths(xPos, yPos, _baseNote));
  }
