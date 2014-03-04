  /*
    Utilities.h
    Created by Amanda Ghassaei, Nov 12, 2013.
  */
  
  #ifndef Utilities_h
  #define Utilities_h
  
  #include "Arduino.h"
  
  //translate button location (x,y) to MIDI note, based on fourths: http://www.youtube.com/watch?v=uQm3xbTxJRc
  byte createMIDINoteInFourths(byte xPos, byte yPos, byte baseNote);
  byte calculateBaseNoteFromPotVal(int val);
  byte velocityFromAnalogVal(int val);
  byte xOffsetFromPotVal(int val);
   
  
  #endif
