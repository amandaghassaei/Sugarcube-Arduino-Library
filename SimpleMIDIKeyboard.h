//  /*
//    SimpleMIDIKeyboard.h
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */
  
  #include "Delegate.h"
  #include "SugarCube.h"
  
  #ifndef SimpleMIDIKeyboard_h
  #define SimpleMIDIKeyboard_h
  
  class SimpleMIDIKeyboard: public Delegate {
    
    public:
    
      SimpleMIDIKeyboard(){}
      
      void buttonPressed(byte xPos, byte yPos);
      void buttonReleased(byte xPos, byte yPos);
      
    private:
    
    
  };
  
  #endif
