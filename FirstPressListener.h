  /*
    FirstPressListener.h
    Created by Amanda Ghassaei, Nov 7, 2013.
  */
  
  #ifndef FirstPressListener_h
  #define FirstPressListener_h
  
  #import "Delegate.h"
    
  class FirstPressListener: public Delegate {
    
    public:
    
      FirstPressListener();
      boolean waitingForFirstPress();
      void buttonPressed(byte xPos, byte yPos);
      byte getFirstPress();
      
    private:
    
//      void doSelectedLEDSequence();
      boolean _stillWaitingForPress;
      byte _firstPress;
    
  };
  
  #endif
