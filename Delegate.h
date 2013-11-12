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
      void virtual XAccHasChanged(int val){}
      void virtual YAccHasChanged(int val){}
      void virtual Pot1HasChanged(int val){}
      void virtual Pot2HasChanged(int val){}
      void virtual XGyroHasChanged(int val){}
      void virtual YGyroHasChanged(int val){}
      
   protected:
   
     SugarCube * _sugarcube;
     
   };
  
  #endif
