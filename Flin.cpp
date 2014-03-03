//  /*
//    Flin.cpp
//    Created by Amanda Ghassaei, Mar 2, 2013.
//  */

  #include "SugarCube.h"
  
  Flin::Flin()
  {
    _pixTimers = 0;
    this->clearAllStorage();
    byte notes[] = {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74};
    for (int i=0;i<16;i++){
      _notes[i] = notes[i];
    }
  }
  
  void Flin::pot1HasChanged(int val)
  {
  }
  
  void Flin::pot2HasChanged(int val)
  {
    _velocity = velocityFromAnalogVal(val);
  }
  
  void Flin::clearAllStorage()
  {
    for (byte i=0;i<16;i++){
      _states[i] = 0;
    }
  }
  
  void Flin::routine100kHz()
  {
    if (_pixTimers++>10){
      for (byte i=0;i<16;i++){
        boolean lsb = _states[i]&1;
        _states[i] = _states[i]>>1;
        if (i<4) _states[i] |= _sugarcube->getStateOfButtonCol(i);
        if (lsb==1) {
          unsigned long msb = 1;
          _states[i] |= (msb<<31);
          if (!(boolean)_states[i]) _sugarcube->noteOff(_notes[i]);
        } else {
          if (_states[i]&1) _sugarcube->noteOn(_notes[i], _velocity);
        }
      }
      _pixTimers = 0;
    }
    for (int i=0;i<4;i++){
      _sugarcube->setLEDCol(i, _states[i]&15);
    }
  }
  
  void Flin::wasShaken()
  {
    for (int i=0;i<16;i++){
      if (_states[i]&1) _sugarcube->noteOff(_notes[i]);
    }
    this->clearAllStorage();
  }

