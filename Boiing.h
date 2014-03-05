//  /*
//    Boiing.h
//    Created by Amanda Ghassaei, Mar 2, 2014.
//  */


  #ifndef Boiing_h
  #define Boiing_h
  
  #include "Delegate.h"
  
  class Boiing: public Delegate {
    
    public:
    
      Boiing();
      
      void pot1HasChanged(int val);//horizontal scroll
      void pot2HasChanged(int val);//tempo
      void wasShaken();//clear
      void buttonPressed(byte xPos, byte yPos);
      
      void routine100kHz();

    private:
      
      byte _notes[16];
      
      boolean _direction[16];//which direction to move
      byte _columnTimers[16];
      byte _maxHeights[16];
      byte _states[16];
      
      void incrementCol(byte colNum);
      int scaleAcc(byte val);
      
      byte _maxTempo;
      byte maxTempoFromPotVal(int val);
      
      void clearAllStorage();
      
      byte _xOffset;
      byte absolutePosition(byte pos);
      byte relativePosition(byte pos);
  };
  
  #endif
