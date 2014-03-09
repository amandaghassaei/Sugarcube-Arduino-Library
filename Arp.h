//  /*
//    Arp.h
//    Created by Amanda Ghassaei, Mar 5, 2014.
//  */


  #ifndef Arp_h
  #define Arp_h
  
  #include "Delegate.h"
  
  class Arp: public Delegate {
    
    public:
    
      Arp();
      
      void pot1HasChanged(int val);//velocity
      void pot2HasChanged(int val);//tempo
      void wasShaken();//clear
      void buttonPressed(byte xPos, byte yPos);
      
      void routine100kHz();

    private:
      
      byte _currentCol;
      void updateCurrentCol();
      
      byte _states[4];
      byte notesActive();
      
      byte _basenote;
      void checkForAcc();
      byte _baseNoteTimer;
      byte _lastNote;//leep track of this so we can turn it off
      
      byte _maxTempo;
      byte _tempoTimer;
      byte maxTempoFromPotVal(int val);
      byte _velocity;
      
      void clearAllStorage();
  };
  
  #endif
