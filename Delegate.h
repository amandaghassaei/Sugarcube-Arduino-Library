  /*
    Delegate.h
    Created by Amanda Ghassaei, Nov 6, 2013.
  */
  
  #ifndef Delegate_h
  #define Delegate_h
  
  #include "Arduino.h"
  #include "Utilities.h"
  
  class SugarCube;
    
  class Delegate//parent class - allows message to be sent from sugarcube class into individual apps
  {
    public:
    
      Delegate(){}//constructor method
      void setSugarCube(SugarCube * sugarcube)
      {
        _sugarcube = sugarcube;
      }

      //buttons
      void virtual buttonPressed(byte xPos, byte yPos){}
      void virtual buttonReleased(byte xPos, byte yPos){}
      void virtual buttonStateChanged(byte xPos, byte yPos, boolean state){}
      void virtual buttonRowChanged(byte row, byte state){}
      void virtual buttonColChanged(byte col, byte state){}
      void virtual buttonStatesChanged(){}
      
      //analog
      void virtual xAccHasChanged(int val){}
      void virtual yAccHasChanged(int val){}
      void virtual pot1HasChanged(int val){}
      void virtual pot2HasChanged(int val){}
      void virtual xGyroHasChanged(int val){}
      void virtual yGyroHasChanged(int val){}
      
      void virtual wasShaken(){}
      
      //100Hz timer routine
      void virtual routine100kHz(){}
      
   protected:
   
     SugarCube * _sugarcube;
     
   };
  
  #endif
