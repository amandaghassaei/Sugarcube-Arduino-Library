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
      
      void wasShaken();

    private:
      
      byte _notes[16];
      void clearAllStorage();
      
      byte _xOffset;
      byte absolutePosition(byte pos);
      byte relativePosition(byte pos);
  };
  
  #endif
