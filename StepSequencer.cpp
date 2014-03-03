//  /*
//    StepSequencer.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */
  
  #include "SugarCube.h"
  
  StepSequencer::StepSequencer()
  {
    _tempoTimer = 0;
    _maxTempo = this->maxTempoFromPotVal(_sugarcube->getPot2Val());
    _playhead = 0;
  }
  
  void StepSequencer::routine100kHz()
  {
    if (_tempoTimer++>_maxTempo){
      _tempoTimer = 0;
      this->incrementPlayhead();
    }
  }
  
  void StepSequencer::pot1HasChanged(int val)
  {
    
  }
  
  void StepSequencer::pot2HasChanged(int val)
  {
    _tempoTimer = maxTempoFromPotVal(val);
  }
  
  byte StepSequencer::maxTempoFromPotVal(int val)//10 bit val
  {
    return (val>>6 + 1)*10;
  }
  
  void StepSequencer::incrementPlayhead()
  {
    _sugarcube->setLEDCol(_playhead, 0);//turn off column
    _playhead++;
    if (_playhead>3) _playhead = 0;
    _sugarcube->setLEDCol(_playhead, 15);//turn on column
  }
