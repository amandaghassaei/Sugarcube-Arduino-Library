//  /*
//    Flin.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */

  #include "SugarCube.h"
  
  Flin::Flin()
  {
    this->clearAllStorage();
    byte notes[] = {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74};
    for (byte i=0;i<16;i++){
      _notes[i] = notes[i];
    }
  }
  
  void Flin::pot1HasChanged(int val)
  {
  }
  
  void Flin::pot2HasChanged(int val)
  {
  }
  
  void Flin::buttonPressed(byte xPos, byte yPos)
  {
    _states[xPos] |= 1<<(3-yPos);
  }
  
  void Flin::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
      _states[i] = 0;
      _columnTimers[i] = 0;
      _columnStepTime[i] = 20;
    }
  }
  
  void Flin::routine100kHz()
  {
    for (byte i=0;i<16;i++){
      this->incrementCol(i);
    }
    for (byte i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _states[i]&15);
    }
  }
  
  void Flin::incrementCol(byte colNum)
  {
    if (_states[colNum]==0) return;
    if (_columnTimers[colNum]++>_columnStepTime[colNum]){
        boolean lsb = _states[colNum]&1;
        _states[colNum] = _states[colNum]>>1;
        if (colNum<4) _states[colNum] |= _sugarcube->getStateOfButtonCol(colNum);
        if (lsb==1) {
          unsigned long msb = 1;
          _states[colNum] |= (msb<<31);
          if (!(boolean)_states[colNum]) _sugarcube->noteOff(_notes[colNum]);
        } else {
          if (_states[colNum]&1) _sugarcube->noteOn(_notes[colNum], 100);
        }
      _columnTimers[colNum] = 0;
    }
  }
  
  void Flin::wasShaken()
  {
    for (byte i=0;i<16;i++){
      if (_states[i]&1) _sugarcube->noteOff(_notes[i]);
    }
    this->clearAllStorage();
  }

