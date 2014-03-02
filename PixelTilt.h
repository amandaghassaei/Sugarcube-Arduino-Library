//  /*
//    PixelTile.h
//    Created by Amanda Ghassaei, Feb 15, 2014.
//  */


  #ifndef PixelTilt_h
  #define PixelTilt_h
  
  #include "Delegate.h"
  
  class PixelTilt: public Delegate {
    
    public:
    
      PixelTilt();
      
      void buttonPressed(byte xPos, byte yPos);
      void buttonReleased(byte xPos, byte yPos);
      
    private:
    
      byte _baseNote;//lowest note in the keyboard
      byte _velocity;//velocity of MIDI notes
  };
  
  #endif
