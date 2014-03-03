//  /*
//    SimpleMIDIKeyboard.h
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */


  #ifndef SimpleMIDIKeyboard_h
  #define SimpleMIDIKeyboard_h
  
  #include "Delegate.h"
  
  class SimpleMIDIKeyboard: public Delegate {
    
    public:
    
      SimpleMIDIKeyboard();
      
      void buttonPressed(byte xPos, byte yPos);
      void buttonReleased(byte xPos, byte yPos);
      void Pot1HasChanged(int val);//base note
      void Pot2HasChanged(int val);//velocity
      void XAccHasChanged(int val);//pitchbend
      
    private:
    
      byte _baseNote;//lowest note in the keyboard
      byte _velocity;//velocity of MIDI notes
  };
  
  #endif
