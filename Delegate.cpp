  /*
    Delegate.cpp
    Created by Amanda Ghassaei, Nov 6, 2013.
  */
  
  #include "Delegate.h"
  
  //---------------------------------------------------------------------
  //-------------------------CONSTRUCTOR/SETUP---------------------------
  //---------------------------------------------------------------------

  Delegate::Delegate()//constructor method
  {
  }
  
  void Delegate::setSugarCube(SugarCube * sugarcube)//constructor method
  {
    _sugarcube = sugarcube;
  }
        
  //buttons
  void Delegate::buttonPressed(byte xPos, byte yPos)
  {
    _sugarcube->SugarCube::turnOnLED(xPos,yPos);
  }
  
  void Delegate::buttonReleased(byte XPos, byte yPos)
  {
  }
  
  void Delegate::buttonStateChanged(byte xPos, byte yPos, boolean state)
  {
  }
  
  void Delegate::buttonRowChanged(byte row, byte state)
  {
  }
  
  void Delegate::buttonColChanged(byte col, byte state)
  {
  }
  
  void Delegate::buttonStatesChangedBitmap(byte * states)
  {
  }
  
  void Delegate::buttonStatesChangedArray(byte * states)
  {
  }
  
  
  //analog
//  Delegate::XAccHasChanged(int val)
//  Delegate::YAccHasChanged(int val)
//  Delegate::Pot1HasChanged(int val)
//  Delegate::Pot2HasChanged(int val)
//  Delegate::XGyroHasChanged(int val)
//  Delegate::YGyroHasChanged(int val)
