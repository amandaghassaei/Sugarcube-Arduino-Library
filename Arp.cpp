//  /*
//    Arp.cpp
//    Created by Amanda Ghassaei, Mar 5, 2013.
//  */

  #include "SugarCube.h"
  
  Arp::Arp()
  {
    this->clearAllStorage();
    _tempoTimer = 0;
    _maxTempo = this->maxTempoFromPotVal(_sugarcube->getPot2Val());
    _velocity = velocityFromAnalogVal(_sugarcube->getPot2Val());
    _baseNoteTimer = 0;
  }
  
  void Arp::pot2HasChanged(int val)
  {
    _maxTempo = this->maxTempoFromPotVal(val);
  }
  
  byte Arp::maxTempoFromPotVal(int val)//10 bit val
  {
    return ((val>>6) + 1)*5;
  }
  
  void Arp::pot1HasChanged(int val)
  {
    _velocity = velocityFromAnalogVal(val);
  }
  
  void Arp::buttonPressed(byte xPos, byte yPos)
  {
    if (_states[xPos]==0){
      _sugarcube->turnOnLED(xPos, yPos);
      _states[xPos] = 1<<(3-yPos);
    } else {
      if (1<<(3-yPos)>_states[xPos]){
        _states[xPos] = _states[xPos]<<1;
      } else if (1<<(3-yPos)<_states[xPos]){
        _states[xPos] = _states[xPos]>>1;
      } else {
        //change pattern
      }
    }
  }
  
  void Arp::checkForAcc()
  {
    byte val = _sugarcube->getXAxisAccVal();//0-127
    if (val<50){
      _basenote-=5;
    } else if (val>76){
      _basenote+=5;
    }
  }
  
  void Arp::clearAllStorage()
  {
    for (byte i=0;i<4;i++){
      _states[i] = 0;
    }
    _currentCol = 0;
    _basenote = 50;
  }
  
  byte Arp::notesActive()
  {
    byte numNotes = 0;
    for (byte i=0;i<4;i++){
      if (_states[i] != 0) numNotes++;
    }
    return numNotes;
  }
  
  void Arp::routine100kHz()
  {
    if (this->notesActive()==0) return;
    if (_tempoTimer++>_maxTempo){
      _tempoTimer = 0;
      this->updateCurrentCol();//increment _currentCol
      
      _sugarcube->noteOff(_lastNote);
      
      //update LEDs
      for (byte i=0;i<4;i++){
        _sugarcube->setLEDCol(i, _states[i]&15);
      }
      if (this->notesActive()!=1) _sugarcube->setLEDCol(_currentCol, 0);
      _lastNote = createMIDINoteInFourths(_currentCol, yCoordFromColState(_states[_currentCol]), _basenote);
      _sugarcube->noteOn(_lastNote, _velocity);
      
      _baseNoteTimer++;
      if (_baseNoteTimer>=2*(this->notesActive())) this->checkForAcc();
    }
  }
  
  void Arp::updateCurrentCol()
  {
    _currentCol++;
    if (_currentCol>3) _currentCol = 0;
    if (_states[_currentCol] != 0) return;
    for (byte i=0;i<4;i++){
      if (_states[i] != 0) {
        _currentCol = i;
        return;
      }
    }
  }
  
  void Arp::wasShaken()
  {
    this->clearAllStorage();
    _sugarcube->clearLEDs();
  }

