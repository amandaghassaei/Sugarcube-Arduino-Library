//  /*
//    SimpleMIDIKeyboard.h
//    Created by Amanda Ghassaei, Mar 2, 2014.
//  */


  #ifndef StepSequencer_h
  #define StepSequencer_h
  
  #include "Delegate.h"
  
  class StepSequencer: public Delegate {
    
    public:
    
      StepSequencer();
      
      void routine100kHz();
      
    private:
    
  };
  
  #endif
