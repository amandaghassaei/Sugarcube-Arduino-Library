//  /*
//    StepSequencer.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */
  
  #include "SugarCube.h"
  
  StepSequencer::StepSequencer()
  {
    _tempoTimer = 0;
    _maxTempo = this->maxTempoFromPotVal(_sugarcube->getPot2Val());
    _xOffset = xOffsetFromPotVal(_sugarcube->getPot1Val());
    _playhead = absolutePosition(15);//start at -1
    
    _velocity = 100;
    //change these to change available notes
    _notes[3] = 72;
    _notes[2] = 67;
    _notes[1] = 63;
    _notes[0] = 60;   
    
    this->clearAllStorage();
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
    byte colState = _seqStates[this->absolutePosition(xPos)];
    if (colState & 1<<(3-yPos)){//if already on
      colState &= ~(1<<(3-yPos));//turn off
      _sugarcube->turnOffLED(xPos, yPos);
    } else {
      colState |= (1<<(3-yPos));//turn on
      _sugarcube->turnOnLED(xPos, yPos);
    }
    _seqStates[this->absolutePosition(xPos)]  = colState;
  }
  
  byte StepSequencer::absolutePosition(byte pos)
  {
    return (pos+_xOffset)&15;
  }
  
  byte StepSequencer::relativePosition()
  {
    return (_playhead+16-_xOffset)&15;
  }
  
  void StepSequencer::pot1HasChanged(int val)
  {
    _xOffset = xOffsetFromPotVal(val);
    for (byte i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _seqStates[this->absolutePosition(i)]);
    }
    if (this->relativePosition()<4) _sugarcube->setLEDCol(this->relativePosition(), 15);//turn on column
  }
  
  void StepSequencer::pot2HasChanged(int val)
  {
    _maxTempo = this->maxTempoFromPotVal(val);
  }
  
  byte StepSequencer::maxTempoFromPotVal(int val)//10 bit val
  {
    return ((val>>6) + 1)*5;
  }
  
  void StepSequencer::incrementPlayhead()
  {
    if (this->relativePosition()<4)_sugarcube->setLEDCol(this->relativePosition(), _seqStates[_playhead]);//set col to original state
    this->playNotesForStates(_seqStates[_playhead], false);
    _playhead++;
    if (_playhead>15) _playhead = 0;
    if (this->relativePosition()<4) _sugarcube->setLEDCol(this->relativePosition(), 15);//turn on column
    this->playNotesForStates(_seqStates[_playhead], true);
  }
  
  void StepSequencer::playNotesForStates(byte column, boolean noteOn)
  {
    for (byte i=0;i<4;i++){
      if (column & 1<<i){
        if (noteOn){
          _sugarcube->noteOn(_notes[i], _velocity);
        } else {
          _sugarcube->noteOff(_notes[i]);
        }
      }
    }
  }
  
  void StepSequencer::wasShaken()
  {
    this->clearAllStorage();
    for (byte i=0;i<4;i++){
      _sugarcube->noteOff(_notes[i]);//turn off any notes that might be stuck on
    }
     _sugarcube->clearLEDs();
     if (this->relativePosition()<4) _sugarcube->setLEDCol(this->relativePosition(), 15);//turn on column
  }
  
  void StepSequencer::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
        _seqStates[i] = 0;
    }
  }
