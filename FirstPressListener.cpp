/*
    FirstPressListener.cpp
    Created by Amanda Ghassaei, Nov 8, 2013.
  */
  
  #include "FirstPressListener.h"
  #include "SugarCube.h"

  FirstPressListener::FirstPressListener()
  {
    _stillWaitingForPress = true;
  }

  boolean FirstPressListener::waitingForFirstPress()
  {
    return _stillWaitingForPress;
  }
  
  byte FirstPressListener::getFirstPress()
  {
    return _firstPress;
  }
  
  void FirstPressListener::buttonPressed(byte xPos, byte yPos)
  {
    if (_stillWaitingForPress) {
      _sugarcube->turnOnLED(xPos,yPos);
      _firstPress = yPos*4 + xPos;
      _stillWaitingForPress = false;
    }
  }
    
