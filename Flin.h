//  /*
//    Flin.h
//    Created by Amanda Ghassaei, Mar 2, 2014.
//  */


  #ifndef Flin_h
  #define Flin_h
  
  #include "Delegate.h"
  
  class Flin: public Delegate {
    
    public:
    
      Flin();
      
      void pot1HasChanged(int val);//horizontal scroll
      void pot2HasChanged(int val);//velocity
      void wasShaken();//clear
      void buttonPressed(byte xPos, byte yPos);
      
      void routine100kHz();
      
      
    private:
    
      unsigned long _states[16];//16x32
      byte _notes[16];
      
      byte _columnTimers[16];
      byte _columnStepTime[16];
      void incrementCol(byte colNum);
      
      void clearAllStorage();
    
  };
  
  #endif
