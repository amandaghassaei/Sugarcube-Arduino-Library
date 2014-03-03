//  /*
//    SimpleMIDIKeyboard.cpp
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */
  
  #include "SugarCube.h"
  
  SimpleMIDIKeyboard::SimpleMIDIKeyboard()
  {
    _baseNote = calculateBaseNoteFromPotVal(_sugarcube->getPot1Val());
    _velocity = velocityFromAnalogVal(_sugarcube->getPot2Val());
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
  
  void SimpleMIDIKeyboard::pot1HasChanged(int val)
  {
    _baseNote = calculateBaseNoteFromPotVal(val);
  }
  
  void SimpleMIDIKeyboard::pot2HasChanged(int val)
  {
    _velocity = velocityFromAnalogVal(val);
  }
  
  void SimpleMIDIKeyboard::xAccHasChanged(int val)
  {
    _sugarcube->pitchBend(val);
  }
