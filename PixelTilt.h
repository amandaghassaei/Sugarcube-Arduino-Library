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
      void Pot1HasChanged(int val);//base note
      void Pot2HasChanged(int val);//velocity
      //tilt
      void routine100kHz();
      
    private:
    
      byte _baseNote;//lowest note in the keyboard
      byte _lastNote;//current note on (in case we change base note after a note has started, but before it has ended
      byte _velocity;//velocity of MIDI notes
      boolean _firstPress;//wait to do stuff until a press is detected
      boolean _pixelHeld;//lets user pin a pixel in place, tilt, and then drop the pixel
      //position of currently lit pixel
      byte _xPos;
      byte _yPos;
      //variables for controlling x and y velocity
      byte _xTimer;
      byte _yTimer;
      void setPixel(byte x, byte y, boolean state);//set state of pix
      
      int getMaxTimerFromAcc(byte val);
      void decrementXPos();
      void incrementXPos();
      void decrementYPos();
      void incrementYPos();
  };
  
  #endif
