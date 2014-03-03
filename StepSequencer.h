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
      void pot1HasChanged(int val);//paging
      void pot2HasChanged(int val);//tempo
      void buttonPressed(byte xPos, byte yPos);
      void wasShaken();
      
    private:
      
      byte _seqStates[16];//storage array for sequencer
    
      byte _tempoTimer;
      byte _maxTempo;
      byte maxTempoFromPotVal(int val);
      
      byte _playhead;//which beat we're currently on
      void incrementPlayhead();
      
      byte _velocity;
      byte _notes[4];
      void clearAllStorage();
      void playNotesForStates(byte column, boolean noteOn);
    
  };
  
  #endif
