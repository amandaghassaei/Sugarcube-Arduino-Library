//  /*
//    SerialComm.h
//    Created by Amanda Ghassaei, March 9, 2014.
//  */


  #ifndef SerialComm_h
  #define SerialComm_h
  
  #include "Delegate.h"
  
  class SerialComm: public Delegate {
    
    public:
    
      SerialComm();
      
      void buttonStateChanged(byte xPos, byte yPos, boolean state);
      void xAccHasChanged(int val);
      void yAccHasChanged(int val);
      void pot1HasChanged(int val);
      void pot2HasChanged(int val);

  };
  
  #endif
