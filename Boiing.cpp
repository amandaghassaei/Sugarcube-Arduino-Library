//  /*
//    Boiing.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */

  #include "SugarCube.h"
  
  Boiing::Boiing()
  {
    this->clearAllStorage();
    _maxTempo = this->maxTempoFromPotVal(_sugarcube->getPot2Val());
    _xOffset = xOffsetFromPotVal(_sugarcube->getPot1Val());
    byte notes[] = {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74};
    for (byte i=0;i<16;i++){
      _notes[i] = notes[i];
    }
  }
  
  void Boiing::pot1HasChanged(int val)
  {
    _xOffset = xOffsetFromPotVal(val);
    for (byte i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _states[this->absolutePosition(i)]&15);
    }
  }
  
  void Boiing::pot2HasChanged(int val)
  {
    _maxTempo = this->maxTempoFromPotVal(val);
  }
  
  byte Boiing::maxTempoFromPotVal(int val)//10 bit val
  {
    return ((val>>6) + 1)*5;
  }
  
  void Boiing::buttonPressed(byte xPos, byte yPos)
  {
    _sugarcube->turnOnLED(xPos, yPos);
    byte absolutePosition = this->absolutePosition(xPos);
    _states[absolutePosition] = 1<<(3-yPos);
    _direction[absolutePosition]  = 0;//head towards bounce
    _maxHeights[absolutePosition] = 1<<(3-yPos);
  }
  
  void Boiing::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
      _states[i] = 0;
      _direction[i] = 0;
      _columnTimers[i] = 0;
      _maxHeights[i] = 0;
    }
  }
  
  void Boiing::routine100kHz()
  {
    for (byte i=0;i<16;i++){
      this->incrementCol(i);
    }
    for (byte i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _states[this->absolutePosition(i)]&15);
    }
  }
  
  void Boiing::incrementCol(byte colNum)
  {
    if (_states[colNum]==0) return;
    if (_columnTimers[colNum]++>_maxTempo){
      _columnTimers[colNum] = 0;
      int scaledAcc = this->scaleAcc(_sugarcube->getYAxisAccVal());
      if (scaledAcc == 0) return;
      
      if (scaledAcc>0){//tilted toward negative y
        if (_direction[colNum]){//heading toward max
          if (_states[colNum] >= _maxHeights[colNum]) {
            _direction[colNum] = 0;
          }
        } else {//heading toward min
          if (_states[colNum]&1) {
            _direction[colNum] = 1;
            _sugarcube->noteOff(_notes[colNum]);
          }
        }
        
        //move pixel
        if (_maxHeights[colNum]==1) return;
        if (_direction[colNum]) { 
          _states[colNum] = _states[colNum]<<1;
        } else {
          _states[colNum] = _states[colNum]>>1;
        }
      
      } else {//tilted toward positive y
        if (_direction[colNum]){//heading toward max
          if (_states[colNum] <= _maxHeights[colNum]) {
            _direction[colNum] = 0;
          }
        } else {//heading toward min
          if (_states[colNum]&(1<<3)) {
            _direction[colNum] = 1;
            _sugarcube->noteOff(_notes[colNum]);
          }
        }
        
        //move pixel
        if (_maxHeights[colNum]==8) return;
        if (_direction[colNum]) { 
          _states[colNum] = _states[colNum]>>1;
        } else {
          _states[colNum] = _states[colNum]<<1;
        }
      }
    }
  }
  
  int Boiing::scaleAcc(byte val)
  {
    byte acc3Bit = val>>4;//scale to 3 bit res (0-7)
    int sign = 1;
    if (acc3Bit == 3 || acc3Bit == 4){
      return 0;
    }
    if (acc3Bit>4){//transform 5, 6, 7 vals to 2, 1, 0
      acc3Bit = 7-acc3Bit;
      sign = -1;
    }
    acc3Bit++;//transform 0, 1, 2 vals to 1, 2, 3
    return sign*acc3Bit;
  }
  
  byte Boiing::absolutePosition(byte pos)
  {
    return (pos+_xOffset)&15;
  }
  
  byte Boiing::relativePosition(byte pos)
  {
    return (pos+16-_xOffset)&15;
  }
  
  void Boiing::wasShaken()
  {
    this->clearAllStorage();
    _sugarcube->clearLEDs();
  }

