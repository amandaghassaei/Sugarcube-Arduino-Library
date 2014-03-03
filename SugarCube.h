  /*
    SugarCube.h
    Created by Amanda Ghassaei, July 20, 2013.
  */
  
  #ifndef SugarCube_h
  #define SugarCube_h
  
  #include "Arduino.h"
  #include "Delegate.h"
  
  #include "SimpleMIDIKeyboard.h"
  #include "PixelTilt.h"
  #include "StepSequencer.h"
  
  class SugarCube
  {
    public:
    
      SugarCube();//constructor method
      
      //set pin connections
      void setLedLatchPin(byte pinNum = 6);
      void setLedClockPin(byte pinNum = 5);
      void setLedDataPin(byte pinNum = 7);
      void setButtonLatchPin(byte pinNum = 4);
      void setButtonClockPin(byte pinNum = 3);
      void setButtonDataPin(byte pinNum = 2);
      void setXAccPin(byte pinNum = A4);
      void setYAccPin(byte pinNum = A3);
      void setPot1Pin(byte pinNum = A2);
      void setPot2Pin(byte pinNum = A5);
      void setXGyroPin(byte pinNum = A1);
      void setYGyroPin(byte pinNum = A0);
      
      byte init();//initialization function, call this in setup()
    
      //button states
      boolean * getButtonStatesArray(boolean storageArray[4][4]);//current button states, 4x4 array of 0's and 1's -> 1= pressed, 0 = not pressed [xPos][yPos]
      byte * getButtonStatesBitmap(boolean byRow, byte storageArray[4]);//set states of all 16 buttons in 4 four bit bitmaps, each element in the returned array is a binary number between 0000 (all released) and 1111 (all pressed), when byRow == TRUE the first element of the returned array corresponds to the first row of leds, the second element of the returned array to the second row, and so on... when byRow == FALSE the first element of the returned array corresponds to the first column of leds, the second element of the returned array to the second column, and so on...
      boolean getStateOfButton(byte xPos, byte yPos);//get state of one button
      byte getStateOfButtonRow(byte row);
      byte getStateOfButtonCol(byte col);
      byte * getPressedButtons(byte storageArray[][2]);//returns a list of the buttons currently pressed [buttonNum][xPos,yPos]
      byte numPressedButtons(byte * pressedButtonList = NULL);//amount of buttons pressed currently

      //accelerometer data
      byte getXAxisAccVal();//returns value of accelerometer x axis, value between 0-127
      byte getYAxisAccVal();//returns value of accelerometer y axis, value between 0-127
      
      //gyroscope data
      int getXAxisGyroVal();//returns value of gyroscope x axis, value between 0-1023
      int getYAxisGyroVal();//returns value of gyroscope y axis, value between 0-1023
      
      //potentiometer data
      int getPot1Val();//returns value of potentiomenter #1, value between 0-1023
      int getPot2Val();//returns value of potentiomenter #2, value between 0-1023
      
      //LED states
      void turnOnLED(byte xPos, byte yPos);//turn on one led
      void turnOffLED(byte xPos, byte yPos);//turn off one led
      void setLEDState(byte xPos, byte yPos, boolean state);//set state of led -> 1 = on, 0 = off
      void setLEDsByArray(boolean  * states);//set states of all 16 leds with 4x4 boolean array states[x][y] -> 1 = on, 0 = off
      void setLEDsByBitmap(byte * states, boolean byRow);//set states of all 16 leds with 4 bitmaps (states[4]), each element in states is a binary number between 0000 (all off) and 1111 (all on), when byRow == TRUE the first element of states corresponds to the first row of leds, the second element of states to the second row, and so on... when byRow == FALSE the first element of states corresponds to the first column of leds, the second element of states to the second column, and so on...
      void setLEDRow(byte row, byte states);//set entire row of four leds, states is a binary number between 0000 and 1111
      void setLEDCol(byte col, byte states);//set entire column of four leds, states is a binary number between 0000 and 1111
      void clearLEDs();//turn off all leds
      
      //send MIDI data
      void noteOn(byte note, byte velocity = 100, byte channel = 0);//turn note on, note = 0-127, , velocity = 0-127, channel  = 0-15, (default velocity = 100, channel = 0)
      void noteOff(byte note, byte channel = 0);//turn note off, note = 0-127, channel  = 0-15 (default channel = 0)
      void sendMIDI(byte command, byte param1, byte param2);//send generic 3 bit MIDI message
      void pitchBend(byte pitchbend, byte channel = 0);//pitchbend = 7 bit pitchbend val (0-127), channel = 0-15 (default channel = 0)
      
      //Timer interrupt routines
      void timer1Routine();
      void timer2Routine();
      
      void setDelegate(Delegate * newDelegate);
      
    private:
    
      //pin connections
      byte _ledLatchPin, _ledClockPin, _ledDataPin;//led pin connections
      byte _buttonLatchPin, _buttonClockPin, _buttonDataPin;//button pin connections
      byte _xAccPin, _yAccPin;//accelerometer pin connections
      byte _pot1Pin, _pot2Pin;//potentiometer pin connections
      byte _xGyroPin, _yGyroPin;//gyroscope pin connections
      
      //debouncing variables
      byte _buttonDebounceTime;//sets the number of counts we will use to determine if a button has been properly debounced
      
      //analog storage
      int _xAccRaw, _yAccRaw, _xGyro, _yGyro, _pot1, _pot2;
      
      //button/led states storage
      byte _ledData[4];//storage for led states, 4 bytes
      byte _buttonCurrent[4], _buttonLast[4], _buttonEvents[4], _buttonStates[4];//storage for buttons, 4 bytes
      byte _buttonDebounceCounter[4][4];//button debounce counter- 16 bytes
      
      //internal setup methods
      void setDefaultPinConnections();
      void setupInputsAndOutputs();
      void timer1Setup();
      void timer2Setup();
      
      //low level button/led/analog controls
      byte _hardwareIter;//iterater for shift(), allows consistant and max led brightness
      void buttonCheck(byte row, byte index);//button debouncer
      void shift(byte i);
      
      void initAnalogPins();
      void checkAnalogPins();
      int analogValFromPin(byte pinNum, int oldVal);
      byte _analogTolerance;
      void setXAcc(int newVal);
      void setYAcc(int newVal);
      byte scaleAcc(int rawVal);
      void setXGyro(int newVal);
      void setYGyro(int newVal);
      void setPot1(int newVal);
      void setPot2(int newVal);
      
      //set output type
      boolean _serialEnabled;
      void setupMIDICommunication();
      void setupSerialCommunication();
      
      //delegate
      Delegate * _delegate;
      byte _delegateTimer;//allows us to send 100Hz interrupt to delegate for time sensitive stuff
      
   };
  
  #endif
