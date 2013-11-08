  /*
    Delegate.cpp
    Created by Amanda Ghassaei, Nov 6, 2013.
  */
  
  #include "Delegate.h"
  #include "SugarCube.h"
  
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
  }
  
  void Delegate::buttonReleased(byte xPos, byte yPos)
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
  
  void Delegate::buttonStatesChanged()
  {
  }
  
  
  //analog
  void Delegate::XAccHasChanged(int val)
  {
  }
  
  void Delegate::YAccHasChanged(int val)
  {
  }
  
  void Delegate::Pot1HasChanged(int val)
  {
  }
  
  void Delegate::Pot2HasChanged(int val)
  {
  }
  
  void Delegate::XGyroHasChanged(int val)
  {
  }
  
  void Delegate::YGyroHasChanged(int val)
  {
  }
  
