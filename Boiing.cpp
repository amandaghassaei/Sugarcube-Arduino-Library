//  /*
//    Boiing.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */

  #include "SugarCube.h"
  
  Boiing::Boiing()
  {
    this->clearAllStorage();
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
  
  void Boiing::buttonPressed(byte xPos, byte yPos)
  {
    _sugarcube->turnOnLED(xPos, yPos);
    byte absolutePosition = this->absolutePosition(xPos);
    _states[absolutePosition] = 1<<(3-yPos);
    _direction[absolutePosition]  = 0;//head down
    _maxHeights[absolutePosition] = 1<<(3-yPos);
    if (_columnStepTime[absolutePosition]==0){
      byte yAcc = _sugarcube->getYAxisAccVal();//0-127
      if (yAcc>50) yAcc = 50;
      _columnStepTime[absolutePosition] = yAcc+3;
    }
  }
  
  void Boiing::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
      _states[i] = 0;
      _direction[i] = 0;
      _columnTimers[i] = 0;
      _columnStepTime[i] = 0;
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
    if (_columnTimers[colNum]++>_columnStepTime[colNum]){
        if (_states[colNum]&1) {
          _sugarcube->noteOff(_notes[colNum]);
          _direction[colNum] = true;
        }
        if (_direction[colNum]){
          _states[colNum] = _states[colNum]<<1;
          if (_states[colNum] >= _maxHeights[colNum]) _direction[colNum] = false;
        } else {
          _states[colNum] = _states[colNum]>>1;
          if (_states[colNum]&1) _sugarcube->noteOn(_notes[colNum], 100);
        }
      _columnTimers[colNum] = 0;
    }
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

