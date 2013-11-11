  #include "SugarCube.h"
  
  SugarCube sugarcube;

  void setup(){
    
    Serial.begin(9600);
    
//    default pin connections are given below:
//
//    Analog
//  
//    0 - Gyroscope Y (Y4.5)
//    1 - Gyroscope X (X4.5)
//    2 - Potentiometer 1 
//    3 - Accelerometer Y (YAcc)
//    4 - Accelerometer X (XAcc)
//    5 - Potentiometer 2
//    
//    Digital
//    
//    0 - serial in - this much remain unconnected
//    1 - serial out - this is hooked up to the MIDI output
//    2 - 74HC165 data pin (Q7)
//    3 - 74HC165 clock pin (CP)
//    4 - 74HC165 latch pin (PL)
//    5 - 74HC595 clock pin (SH_CP)
//    6 - 74HC595 latch pin (ST_CP)
//    7 - 74HC595 data pin (DS)
//    
//    set custom pin connections using the following commands, this must happen before sugarcube.init()
//    leave digital pins 0 and 1 (RX/TX) empty
//    sugarcube.setLedLatchPin(6);
//    sugarcube.setLedClockPin(5);
//    sugarcube.setLedDataPin(7);
//    sugarcube.setButtonLatchPin(4);
//    sugarcube.setButtonClockPin(3);
//    sugarcube.setButtonDataPin(2);
//    sugarcube.setXAccPin(A4);
//    sugarcube.setYAccPin(A3);
//    sugarcube.setPot1Pin(A2);
//    sugarcube.setPot2Pin(A5);
//    sugarcube.setXGyroPin(A1);
//    sugarcube.setYGyroPin(A0);
    
    byte patchNum = sugarcube.init();
    Serial.println(patchNum);
    Delegate * currentApp;
    switch(patchNum) {
      case 0:
      {
        SimpleMIDIKeyboard simpleMIDIKeyboard;
        currentApp = &simpleMIDIKeyboard;
        sugarcube.setDelegate(&simpleMIDIKeyboard);
        break;
      }
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
      case 10:
        break;
      case 11:
        break;
      case 12:
        break;
      case 13:
        break;
      case 14:
        break;
      case 15:
        break;
    }
  }
  
  void loop(){
    
  }
  
  //---------------------------------------------------------------------
  //--------------------INTERRUPT ROUTINES-------------------------------
  //---------------------------------------------------------------------
  
  ISR(TIMER1_COMPA_vect) {//time 1 interrupt, at freq of 1kHz
      sugarcube.timer1Routine();
  }

  ISR(TIMER2_COMPA_vect) {//timer 2 interupt, every 128us
      sugarcube.timer2Routine();
  } 
