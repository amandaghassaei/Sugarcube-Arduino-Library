//  /*
//    Flin.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */

  #include "SugarCube.h"
  
  Flin::Flin()
  {
    this->clearAllStorage();
    _xOffset = xOffsetFromPotVal(_sugarcube->getPot1Val());
    byte notes[] = {55, 57, 60, 62, 65, 67, 69, 72, 74, 77, 79, 81, 84, 86, 89, 91};
    for (byte i=0;i<16;i++){
      _notes[i] = notes[i];
    }
  }
  
  void Flin::pot1HasChanged(int val)
  {
    _xOffset = xOffsetFromPotVal(val);
    for (byte i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _states[this->absolutePosition(i)]&15);
    }
  }
  
  void Flin::buttonPressed(byte xPos, byte yPos)
  {
    _sugarcube->turnOnLED(xPos, yPos);
    byte absolutePosition = this->absolutePosition(xPos);
    _states[absolutePosition] |= 1<<(3-yPos);
    if (_columnStepTime[absolutePosition]==0){
      byte yAcc = _sugarcube->getYAxisAccVal();//0-127
      if (yAcc>50) yAcc = 50;
      _columnStepTime[absolutePosition] = yAcc+3;
    }
  }
  
  void Flin::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
      _states[i] = 0;
      _columnTimers[i] = 0;
      _columnStepTime[i] = 0;
    }
  }
  
  void Flin::routine100kHz()
  {
    for (byte i=0;i<16;i++){
      this->incrementCol(i);
    }
    for (byte i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _states[this->absolutePosition(i)]&15);
    }
  }
  
  void Flin::incrementCol(byte colNum)
  {
    if (_states[colNum]==0) return;
    if (_columnTimers[colNum]++>_columnStepTime[colNum]){
        boolean lsb = _states[colNum]&1;
        _states[colNum] = _states[colNum]>>1;
        if (this->relativePosition(colNum)<4) _states[colNum] |= _sugarcube->getStateOfButtonCol(this->relativePosition(colNum));
        if (lsb==1) {
          unsigned long msb = 1;
          _states[colNum] |= (msb<<31);
          if (!(boolean)_states[colNum]) _sugarcube->noteOff(_notes[colNum]);
        } else {
          if (_states[colNum]&1) _sugarcube->noteOn(_notes[colNum], constrain(127-_columnStepTime[colNum], 110, 127));
        }
      _columnTimers[colNum] = 0;
    }
  }
  
  byte Flin::absolutePosition(byte pos)
  {
    return (pos+_xOffset)&15;
  }
  
  byte Flin::relativePosition(byte pos)
  {
    return (pos+16-_xOffset)&15;
  }
  
  void Flin::wasShaken()
  {
    for (byte i=0;i<16;i++){
      if (_states[i]&1) _sugarcube->noteOff(_notes[i]);
    }
    this->clearAllStorage();
    _sugarcube->clearLEDs();
  }

