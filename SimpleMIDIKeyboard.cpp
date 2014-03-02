//  /*
//    SimpleMIDIKeyboard.cpp
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */
  
  #include "SugarCube.h"
  
  SimpleMIDIKeyboard::SimpleMIDIKeyboard()
  {
    _baseNote = this->calculateBaseNoteFromPotVal(_sugarcube->getPot1Val());
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
  
  void SimpleMIDIKeyboard::Pot1HasChanged(int val)
  {
    _baseNote = calculateBaseNoteFromPotVal(val);
  }
  
  void SimpleMIDIKeyboard::Pot2HasChanged(int val)
  {
    _velocity = velocityFromAnalogVal(val);
  }
  
  byte SimpleMIDIKeyboard::calculateBaseNoteFromPotVal(int val)
  {
    return byte(25+val>>5);//returns #s between 25 and 88
  }
  
  void SimpleMIDIKeyboard::XAccHasChanged(int val)
  {
    _sugarcube->pitchBend(val);
  }
