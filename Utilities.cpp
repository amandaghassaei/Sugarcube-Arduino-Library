//  /*
//    Utilities.cpp
//    Created by Amanda Ghassaei, Nov 12, 2013.
//  */

  #include "Utilities.h"
  #include "Arduino.h"
  
  //translate button location (x,y) to MIDI note, based on fourths: http://www.youtube.com/watch?v=uQm3xbTxJRc
  byte createMIDINoteInFourths(byte xPos, byte yPos, byte baseNote)
  {
    return 5*xPos+yPos+baseNote;
  }
  
  byte calculateBaseNoteFromPotVal(int val)
  {
    return byte(25+(val>>5));//returns #s between 25 and 56
  }
  
  byte velocityFromAnalogVal(int val)
  {
    byte velocity = val>>3;
    return constrain(velocity, 10, 127);//constrain velocity to be at least 10
  }
  
  byte xOffsetFromPotVal(int val)
  {
    return val>>6;
  }
