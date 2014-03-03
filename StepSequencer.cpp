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
    for (byte i=0;i<16;i++){
        _seqStates[i] = 0;
    }
    _velocity = 100;
    //change these to change available notes
    _notes[0] = 60;
    _notes[0] = 63;
    _notes[0] = 67;
    _notes[0] = 72;
  }
  
  void StepSequencer::routine100kHz()
  {
    if (_tempoTimer++>_maxTempo){
      _tempoTimer = 0;
      this->incrementPlayhead();
    }
  }
  
  void StepSequencer::buttonPressed(byte xPos, byte yPos)
  {
    byte colState = _seqStates[xPos];
    if (colState & 1<<(3-yPos)){//if already on
      colState &= ~(1<<(3-yPos));//turn off
      _sugarcube->turnOffLED(xPos, yPos);
    } else {
      colState |= (1<<(3-yPos));//turn on
      _sugarcube->turnOnLED(xPos, yPos);
    }
    _seqStates[xPos]  = colState;
  }
  
  void StepSequencer::pot1HasChanged(int val)
  {
    
  }
  
  void StepSequencer::pot2HasChanged(int val)
  {
    _maxTempo = 100;
//    _maxTempo = maxTempoFromPotVal(val);
  }
  
  byte StepSequencer::maxTempoFromPotVal(int val)//10 bit val
  {
    return (val>>6 + 1)*5;
  }
  
  void StepSequencer::incrementPlayhead()
  {
    _sugarcube->setLEDCol(_playhead, _seqStates[_playhead]);//set col to original state
    this->playNotesForStates(_seqStates[_playhead], false);
    _playhead++;
    if (_playhead>3) _playhead = 0;
    _sugarcube->setLEDCol(_playhead, 15);//turn on column
    this->playNotesForStates(_seqStates[_playhead], true);
  }
  
  void StepSequencer::playNotesForStates(byte column, boolean noteOn)
  {
    for (byte i=0;i<4;i++){
      if (column & 1<<1){
        if (noteOn){
          _sugarcube->noteOn(_notes[i], _velocity);
        } else {
          _sugarcube->noteOff(_notes[i]);
        }
      }
    }
  }
