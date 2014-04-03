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
        _sugarcube->setLEDCol(xPos, _states[xPos]&15);
      } else if (1<<(3-yPos)<_states[xPos]){
        _states[xPos] = _states[xPos]>>1;
        _sugarcube->setLEDCol(xPos, _states[xPos]&15);
      } else {
        byte numNotes = this->notesActive();
        if (numNotes<3) return;
        if (xPos == 0 || (xPos == 1 && _states[0]==0)) {
          this->setIncreasingPattern();
        } else if (xPos == 3 || (xPos == 2 && _states[3]==0)) {
          this->setDecreasingPattern();
        } else {
          this->setRandomPattern();
        }
      }
    }
  }
  
  void Arp::checkForAcc()
  {
    byte val = _sugarcube->getXAxisAccVal();//0-127
    if (val<20){
      _basenote-=5;
    } else if (val>106){
      _basenote+=5;
    }
  }
  
  void Arp::clearAllStorage()
  {
    for (byte i=0;i<4;i++){
      _states[i] = 0;
      _arpPattern[i] = i;
    }
    _arpPatternIndex = 0;
    _currentCol = _arpPattern[_arpPatternIndex];
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
      
      //update LEDs
      for (byte i=0;i<4;i++){
        _sugarcube->setLEDCol(i, _states[i]&15);
      }
      if (this->notesActive()!=1) _sugarcube->setLEDCol(_currentCol, 0);//turn off current note
      
      //MIDI
      _sugarcube->noteOff(_lastNote);
      _lastNote = createMIDINoteInFourths(_currentCol, yCoordFromColState(_states[_currentCol]), _basenote);
      _sugarcube->noteOn(_lastNote, _velocity);
      
      _baseNoteTimer++;
      if (_baseNoteTimer>=2*(this->notesActive())) this->checkForAcc();
    }
  }
  
  void Arp::setIncreasingPattern()
  {
    for (byte i=0;i<4;i++){
      _arpPattern[i] = i;
    }
  }
  
  void Arp::setDecreasingPattern()
  {
    for (byte i=0;i<4;i++){
      _arpPattern[i] = 3-i;
    }
  }
  
  void Arp::setRandomPattern()
  {
    byte newPattern[] = {4, 4, 4, 4};
    for (byte i=0;i<4;i++){
      newPattern[i] = this->getRandomNumber(newPattern, random(4));
    }
    memcpy(_arpPattern, newPattern, 4*sizeof(byte));
  }
  
  byte Arp::getRandomNumber(byte pattern[4], byte newNumber)
  {
    for (byte i=0;i<4;i++){
      if (pattern[i] == newNumber){
        newNumber++;
        if (newNumber>3) newNumber = 0;
        return this->getRandomNumber(pattern, newNumber);
      }
    }
    return newNumber;
  }
  
  void Arp::updateCurrentCol()
  {
    _arpPatternIndex++;
    if (_arpPatternIndex>3)  _arpPatternIndex = 0;
    _currentCol = _arpPattern[_arpPatternIndex];
    if (_states[_currentCol] != 0) return;
    return this->updateCurrentCol();
  }
  
  void Arp::wasShaken()
  {
    this->clearAllStorage();
    _sugarcube->clearLEDs();
  }

