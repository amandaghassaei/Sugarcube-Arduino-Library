  /*
    Delegate.h
    Created by Amanda Ghassaei, Nov 6, 2013.
  */
  
  #ifndef Delegate_h
  #define Delegate_h
  
  #include "Arduino.h"
  
  class SugarCube;
    
  class Delegate//parent class - allows message to be sent from sugarcube class into individual apps
  {
    public:
    
      Delegate();//constructor method
      void setSugarCube(SugarCube * sugarcube);

      //buttons
      void buttonPressed(byte xPos, byte yPos);
      void buttonReleased(byte xPos, byte yPos);
      void buttonStateChanged(byte xPos, byte yPos, boolean state);
      void buttonRowChanged(byte row, byte state);
      void buttonColChanged(byte col, byte state);
      void buttonStatesChanged();
      
      //analog
      void XAccHasChanged(int val);
      void YAccHasChanged(int val);
      void Pot1HasChanged(int val);
      void Pot2HasChanged(int val);
      void XGyroHasChanged(int val);
      void YGyroHasChanged(int val);
      
//   protected:
   
     SugarCube * _sugarcube;
     
   };
  
  #endif
