//  /*
//    PixelTilt.cpp
//    Created by Amanda Ghassaei, Nov 11, 2013.
//  */
  
  #include "SugarCube.h"
  
  PixelTilt::PixelTilt()
  {
    _baseNote = calculateBaseNoteFromPotVal(_sugarcube->getPot1Val());
    _velocity = velocityFromAnalogVal(_sugarcube->getPot2Val());
    _pixelHeld = false;
    _firstPress = false;
    _xTimer = 0;
    _yTimer = 0;
  }
  
  void PixelTilt::buttonPressed(byte xPos, byte yPos)
  {
    _firstPress = true;
    this->setPixel(_xPos, _yPos, false);
    _pixelHeld = true;
    _xPos = xPos;
    _yPos = yPos;
    this->setPixel(_xPos, _yPos, true);
  }
  
  void PixelTilt::buttonReleased(byte xPos, byte yPos)
  {
    _pixelHeld = false;
  }
  
  void PixelTilt::Pot1HasChanged(int val)
  {
    _baseNote = calculateBaseNoteFromPotVal(val);
  }
  
  void PixelTilt::Pot2HasChanged(int val)
  {
    _velocity = velocityFromAnalogVal(val);
  }
  
  void PixelTilt::routine100kHz()
  {
    if (!_firstPress) return;
    if (_pixelHeld) return;
    int xTimerMax = this->getMaxTimerFromAcc(_sugarcube->getXAxisAccVal());
    int yTimerMax = this->getMaxTimerFromAcc(_sugarcube->getYAxisAccVal());
    if ((_xTimer++>abs(xTimerMax)) && (xTimerMax!=0)){
      _xTimer = 0;
      if (xTimerMax<0){
        this->incrementXPos();
      } else {
        this->decrementXPos();
      }
    }
    if ((_yTimer++>abs(yTimerMax)) && (yTimerMax!=0)){
      _yTimer = 0;
      if (yTimerMax<0){
        this->decrementYPos();
      } else {
        this->incrementYPos();
      }
    }
  }
  
  int PixelTilt::getMaxTimerFromAcc(byte val)
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
    return sign*acc3Bit*5;
  }
  
  void PixelTilt::decrementXPos()
  {
    if (_xPos>0) {
      this->setPixel(_xPos, _yPos, false);
      _xPos--;
      this->setPixel(_xPos, _yPos, true);
    }
  }
  
  void PixelTilt::incrementXPos()
  {
    if (_xPos<3) {
      this->setPixel(_xPos, _yPos, false);
      _xPos++;
      this->setPixel(_xPos, _yPos, true);
    }
  }
  
  void PixelTilt::decrementYPos()
  {
    if (_yPos>0) {
      this->setPixel(_xPos, _yPos, false);
      _yPos--;
      this->setPixel(_xPos, _yPos, true);
    }
  }
  
  void PixelTilt::incrementYPos()
  {
    if (_yPos<3) {
      this->setPixel(_xPos, _yPos, false);
      _yPos++;
      this->setPixel(_xPos, _yPos, true);
    }
  }
    
  void PixelTilt::setPixel(byte x, byte y, boolean state)
  {
    _sugarcube->setLEDState(x, y, state);
    if (state) {
      _lastNote = createMIDINoteInFourths(x, y, _baseNote);
      _sugarcube->noteOn(_lastNote, _velocity);
    } else {
      _sugarcube->noteOff(_lastNote);
    }
  }
