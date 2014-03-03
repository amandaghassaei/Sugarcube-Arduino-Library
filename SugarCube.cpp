  /*
    SugarCube.cpp
    Created by Amanda Ghassaei, July 20, 2013.
  */
  
    #include "SugarCube.h"
    #include "FirstPressListener.h"
      
  //---------------------------------------------------------------------
  //-------------------------CONSTRUCTOR/SETUP---------------------------
  //---------------------------------------------------------------------

  SugarCube::SugarCube()
  {
    //constructor method
    this->setDefaultPinConnections();
    
    _buttonDebounceTime = 12;
    _analogTolerance = 5;
    _serialEnabled = false;
    _delegateTimer = 0;
    _shakeDebounceTimer = 0;
    _numShakeExtremes = 0;
    
    //setup button/led states
    for (byte i=0;i<4;i++){
      _ledData[i] = 0;
      _buttonCurrent[i] = 0;
      _buttonLast[i] = 0;
      _buttonEvents[i] = 0;
      _buttonStates[i] = 0;
      for (byte j=0;j<4;j++){
        _buttonDebounceCounter[i][j] = _buttonDebounceTime;
      }
    }
    
    _delegate->setSugarCube(this);
  }
  
  byte SugarCube::init()
  {   
    this->setupInputsAndOutputs();
    this->initAnalogPins();
    this->setupMIDICommunication();
    this->timer1Setup();
    
    FirstPressListener firstPressListener;
    this->setDelegate(&firstPressListener);
    while (firstPressListener.waitingForFirstPress()) {
    }
    delay(1000);
    this->clearLEDs();
    return firstPressListener.getFirstPress();
  }
  
  void SugarCube::setupInputsAndOutputs()
  {
    pinMode(_ledLatchPin,OUTPUT);
    pinMode(_ledClockPin,OUTPUT);
    pinMode(_ledDataPin,OUTPUT);
    pinMode(_buttonLatchPin,OUTPUT);
    pinMode(_buttonClockPin,OUTPUT);
    pinMode(_buttonDataPin,INPUT);
    pinMode(_xAccPin,INPUT);
    pinMode(_yAccPin,INPUT);
    pinMode(_pot1Pin,INPUT);
    pinMode(_pot2Pin,INPUT);
    pinMode(_xGyroPin,INPUT);
    pinMode(_yGyroPin,INPUT);
    //set serial pin connections
    pinMode(1,OUTPUT);
    pinMode(0,INPUT);
//    DDRD = 0xFA;//set pins D7-D4 as output, D2 as input
  }
  
  void SugarCube::timer1Setup()
  {
    _hardwareIter = 0;
    
    cli();//stop interrupts

    //set timer1 interrupt at 1kHz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0;
    // set timer count for 1khz increments
    OCR1A = 1999;// = (16*10^6) / (1000*8) - 1
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS11 bit for 8 prescaler
    TCCR1B |= (1 << CS11);   
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    sei();//allow interrupts
  }
  
  void SugarCube::timer2Setup()//this is only setup for serial communication, not for midi
  { 
    cli();//stop interrupts

    //set timer2 interrupt every 128us
    TCCR2A = 0;// set entire TCCR2A register to 0
    TCCR2B = 0;// same for TCCR2B
    TCNT2  = 0;//initialize counter value to 0
    // set compare match register for 7.8khz increments
    OCR2A = 255;// = (16*10^6) / (7812.5*8) - 1 (must be <256)
    // turn on CTC mode
    TCCR2A |= (1 << WGM21);
    // Set CS11 bit for 8 prescaler
    TCCR2B |= (1 << CS11);   
    // enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);
    
    sei();//allow interrupts
  }
  
  //---------------------------------------------------------------------
  //--------------------PIN CONNECTION SETTERS---------------------------
  //---------------------------------------------------------------------
  
  void SugarCube::setDefaultPinConnections()
  {
    this->setLedLatchPin();
    this->setLedClockPin();
    this->setLedDataPin();
    this->setButtonLatchPin();
    this->setButtonClockPin();
    this->setButtonDataPin();
    this->setXAccPin();
    this->setYAccPin();
    this->setPot1Pin();
    this->setPot2Pin();
    this->setXGyroPin();
    this->setYGyroPin();
  }
  
  void SugarCube::setLedLatchPin(byte pinNum)
  {
    _ledLatchPin = pinNum;
  }
  
  void SugarCube::setLedClockPin(byte pinNum)
  {
    _ledClockPin = pinNum;
  }
  
  void SugarCube::setLedDataPin(byte pinNum)
  {
    _ledDataPin = pinNum;
  }
  
  void SugarCube::setButtonLatchPin(byte pinNum)
  {
    _buttonLatchPin = pinNum;
  }
  
  void SugarCube::setButtonClockPin(byte pinNum)
  {
    _buttonClockPin = pinNum;
  }
  
  void SugarCube::setButtonDataPin(byte pinNum)
  {
    _buttonDataPin = pinNum;
  }
  
  void SugarCube::setXAccPin(byte pinNum)
  {
    _xAccPin = pinNum;
  }
  
  void SugarCube::setYAccPin(byte pinNum)
  {
    _yAccPin = pinNum;
  }
  
  void SugarCube::setPot1Pin(byte pinNum)
  {
    _pot1Pin = pinNum;
  }
  
  void SugarCube::setPot2Pin(byte pinNum)
  {
    _pot2Pin = pinNum;
  }
  
  void SugarCube::setXGyroPin(byte pinNum)
  {
    _xGyroPin = pinNum;
  }
  
  void SugarCube::setYGyroPin(byte pinNum)
  {
    _yGyroPin = pinNum;
  }
  
  //---------------------------------------------------------------------
  //-----------------------RECEIVE BUTTON DATA---------------------------
  //---------------------------------------------------------------------
  
  boolean * SugarCube::getButtonStatesArray(boolean storageArray[4][4])
  {//current button states, 4x4 array of 0's and 1's -> 1= pressed, 0 = not pressed [xPos][yPos]
    byte buttonStateCopy[4];//make a copy in case _buttonStates changes while we are extracting data, things could get confusing
    memcpy(buttonStateCopy, _buttonStates, 4*sizeof(byte));
    for (byte y=0;y<4;y++){
      for (byte x=0;x<4;x++){
        if ((buttonStateCopy[y]>>(3-x))&1){
          storageArray[y][x] = 1;//it's kind of weird to do [y][x], but this is more in line with the way the data is stored in memory
        } else {
          storageArray[y][x] = 0;
        }
      }
    }
    return *storageArray;
  }
  
  byte * SugarCube::getButtonStatesBitmap(boolean byRow, byte storageArray[4])
  {//get states of all 16 buttons in 4 four bit bitmaps, each element in the returned array is a binary number between 0000 (all released) and 1111 (all pressed), when byRow == TRUE the first element of the returned array corresponds to the first row of leds, the second element of the returned array to the second row, and so on... when byRow == FALSE the first element of the returned array corresponds to the first column of leds, the second element of the returned array to the second column, and so on...
    memcpy(storageArray, _buttonStates, 4*sizeof(byte));
    if (byRow){
      return storageArray;
    } else {
      byte statesCopy[4];
      memcpy(statesCopy, storageArray, 4*sizeof(byte));
      for (byte x=0;x<4;x++){
        byte colBitmap = 0;
        for (byte y=0;y<4;y++){
          boolean state = (statesCopy[y]>>(3-x))&1;
          if (state){
            colBitmap |= 1<<(3-y);
          }
        }
        storageArray[x] = colBitmap;
      }
      return storageArray;
    }
  }
  
  boolean SugarCube::getStateOfButton(byte xPos, byte yPos)
  {
    if ((_buttonStates[yPos]>>(3-xPos))&1==1){
      return true;
    }
    return false;
  }
  
  byte SugarCube::getStateOfButtonRow(byte row)
  {
    return _buttonStates[row];
  }
  
  byte SugarCube::getStateOfButtonCol(byte col)
  {
    byte buttonStatesCopy[4];//make a copy in case _buttonStates changes while we are extracting data, things could get confusing
    memcpy(buttonStatesCopy, _buttonStates, 4*sizeof(byte));
    byte colStates = 0;
    for (byte y=0;y<4;y++){
      if ((buttonStatesCopy[y]>>(3-col))&1){
        colStates |= 1<<(3-y);
      }
    }
    return colStates; 
  }
  
  byte * SugarCube::getPressedButtons(byte storageArray[][2])
  {//returns a list of the buttons currently pressed [buttonNum][xPos,yPos]
    //since I cannot import vector in arduino, I have to figure out the size of the array before I build it
    byte buttonStatesCopy[4];//make a copy in case _buttonStates changes while we are extracting data, things could get confusing
    memcpy(buttonStatesCopy, _buttonStates, 4*sizeof(byte));
    byte numPressed = 0;
    for (byte y=0;y<4;y++){
      for (byte x=0;x<4;x++){
        if ((buttonStatesCopy[y]>>(3-x))&1){
          storageArray[numPressed][0] = x;
          storageArray[numPressed][1] = y;
          numPressed++;
        }
      }
    }
    return *storageArray;
  }
  
  byte SugarCube::numPressedButtons(byte * pressedButtonList)//optional argument
  {//amount of buttons pressed currently
    byte numPressed = 0;
    if (pressedButtonList == NULL){
      byte buttonStatesCopy[4];
      memcpy(buttonStatesCopy, _buttonStates, 4*sizeof(byte));
      for (byte y=0;y<4;y++){
        for (byte x=0;x<4;x++){
          if ((buttonStatesCopy[y]>>(3-x))&1){
            numPressed++;
          }
        }
      }
      return numPressed;
    } else {
      for (numPressed=0;numPressed<16;numPressed++){
        if (*(pressedButtonList+2*numPressed) > 4){
          return numPressed;
        }
      }
      return numPressed;
    }
  }

  //---------------------------------------------------------------------
  //------------------------GET ANALOG DATA------------------------------
  //---------------------------------------------------------------------

  byte SugarCube::getXAxisAccVal()
  {//returns value of accelerometer x axis
    return this->scaleAcc(_xAccRaw);
  }
  
  byte SugarCube::getYAxisAccVal()
  {//returns value of accelerometer y axis
    return this->scaleAcc(_yAccRaw);
  }
  
  int SugarCube::getXAxisGyroVal()
  {//returns value of gyroscope x axis
    return _xGyro;
  }
  
  int SugarCube::getYAxisGyroVal()
  {//returns value of gyroscope y axis
    return _yGyro;
  }
  
  int SugarCube::getPot1Val()
  {//returns value of potentiomenter #1
    return _pot1;
  }
  
  int SugarCube::getPot2Val()
  {//returns value of potentiomenter #2
    return _pot1;
  }
  
  //---------------------------------------------------------------------
  //------------------------SEND LED DATA--------------------------------
  //---------------------------------------------------------------------
  
  void SugarCube::turnOnLED(byte xPos, byte yPos)
  {//turn on one led
    _ledData[yPos] |= 1<<(3-xPos);
  }
  
  void SugarCube::turnOffLED(byte xPos, byte yPos)
  {//turn off one led
    _ledData[yPos] &= ~(1<<(3-xPos));
  }
  
  void SugarCube::setLEDState(byte xPos, byte yPos, boolean state)
  {//set state of led -> 1 = on, 0 = off
    if (state) {
      this->turnOnLED(xPos, yPos);
    } else {
      this->turnOffLED(xPos,yPos);
    }
  }
  
  void SugarCube::setLEDsByArray(boolean * states)
  {//set states of all 16 leds with 4x4 boolean array states[x][y] -> 1 = on, 0 = off
    for (byte y=0;y<4;y++){
      byte statesBitmap = 0;
      for (byte x=0;x<4;x++){
        if (*(states + 4*y + x)){
          statesBitmap |= 1<<(3-x);
        }
      }
      this->setLEDRow(y,statesBitmap);
    }
  }
  
  void SugarCube::setLEDsByBitmap(byte * states, boolean byRow)
  {//set states of all 16 leds with 4 bitmaps (states), each element in states is a binary number between 0000 (all off) and 1111 (all on)
  //when byRow == TRUE the first element of states corresponds to the first row of leds, the second element of states to the second row, and so on... 
  //when byRow == FALSE the first element of states corresponds to the first column of leds, the second element of states to the second column, and so on...
    if (byRow){
      memcpy(_ledData, states, 4);
    } else {
      for (byte x=0;x<4;x++){
        byte rowBitMap = 0;
        for (byte y=0;y<4;y++){
          boolean state = ((*(states + y))>>(3-x))&1;//;  (*(pressedButtonList+2*numPressed)
          if (state){
            rowBitMap |= 1<<(3-y);
          }
        }
        this->setLEDRow(x,rowBitMap);
      }
    }
  }
  
  void SugarCube::setLEDRow(byte row, byte states)
  {//set entire row of four leds, states is a binary number between 0000 and 1111
    _ledData[row] = states;
  }
  
  void SugarCube::setLEDCol(byte col, byte states)
  {//set entire column of four leds, states is a binary number between 0000 and 1111
    for (byte y=0;y<4;y++){
      boolean state = (states>>(3-y))&1;
      this->setLEDState(col, y, state);
    }
  }
  
  void SugarCube::clearLEDs()
  {
    for (int i=0;i<4;i++){
      _ledData[i] = 0;
    }
  }
  
  //---------------------------------------------------------------------
  //--------------LOW LEVEL LED/BUTTON MONITORING------------------------
  //---------------------------------------------------------------------
  
  void SugarCube::shift(byte i)
  {
    _buttonLast[i] = _buttonCurrent[i];
    byte dataToSend = (1 << (i+4)) | (15 & ~_ledData[i]);
    // set latch pin low so the LEDs don't change while sending in bits
    PORTD&=B10111111;//digitalWrite(ledLatchPin, LOW);
    // shift out the bits of dataToSend 
    //shiftOut(ledDataPin, ledClockPin, LSBFIRST, dataToSend);  
    for (byte j=0;j<8;j++){
      PORTD&=B11011111;//digitalWrite(ledClockPin,LOW);
      //digitalWrite(ledDataPin,((dataToSend>>j)&1));
      if ((dataToSend>>j)&1){
        PORTD|=B10000000;
      }
      else{
        PORTD&=B01111111;
      }
      PORTD|=B00100000;//digitalWrite(ledClockPin,HIGH);
    }
    //set latch pin high so the LEDs will receive new data
    PORTD|=B01000000;//digitalWrite(ledLatchPin, HIGH);
    
    // SlowDown is put in here to waste a little time while we wait for the state of the output
    // pins to settle.  Without this time wasting loop, a single button press would show up as
    // two presses (the button and its neighbour)
    volatile int SlowDown = 0; 
    while (SlowDown < 15) 
    { 
      SlowDown++; 
    } 
      
    //once one row has been set high, receive data from buttons
    //set latch pin high
    PORTD|=B00010000;//digitalWrite(buttonLatchPin, HIGH);
    //shift in data
    //buttonCurrent[i] = shiftIn(buttonDataPin, buttonClockPin, LSBFIRST) >> 3;
    for (byte j=0;j<4;j++){
      PORTD&=B11110111;//digitalWrite(buttonClockPin,LOW);
      PORTD|=B00001000;//digitalWrite(buttonClockPin,HIGH);
    }
    for (byte j=0;j<4;j++){
      PORTD&=B11110111;//digitalWrite(buttonClockPin,LOW);
      if ((PIND>>2)&1){//digitalRead(buttonDataPin)
        _buttonCurrent[i]|=1<<j;
      }
      else{
        _buttonCurrent[i]&=~(1<<j); 
      }
      PORTD|=B00001000;//digitalWrite(buttonClockPin,HIGH);
    }
    //latchpin low
    PORTD&=B11101111;//digitalWrite(buttonLatchPin, LOW);
    
    for (byte j=0;j<4;j++){
      this->buttonCheck(i,j);
      
      if (_serialEnabled){
        if (_buttonEvents[i]<<j){
          if (_buttonStates[i]&1<<j){
            Serial.write(((3-j)<<3)+(i<<1)+1);
          }
          else{
            Serial.write(((3-j)<<3)+(i<<1)+0);
          }
          _buttonEvents[i] &= ~(1<<j);
        }
      } else {
        if (_buttonEvents[i]<<j){
          boolean state = _buttonStates[i]&1<<j;
          _delegate->buttonStateChanged(3-j,i,state);
          _delegate->buttonRowChanged(i,_buttonStates[i]);
          _delegate->buttonColChanged(3-j,this->getStateOfButtonCol(3-j));
          _delegate->buttonStatesChanged();
          if (state){
            _delegate->buttonPressed(3-j,i);
          }
          else{
            _delegate->buttonReleased(3-j,i);
          }
          _buttonEvents[i] &= ~(1<<j);
        }
      }
    }    
  }
  
  void SugarCube::buttonCheck(byte row, byte index)//check the state of a given button and debounce.
  //this buttoncheck function is largely copied from the monome 40h firmware by brian crabtree and joe lake
  {
    if (((_buttonCurrent[row] ^ _buttonLast[row]) & (1 << index)) &&// if the current physical button state is different from the
    ((_buttonCurrent[row] ^ _buttonStates[row]) & (1 << index))) {// last physical button state AND the current debounced state
      if (_buttonCurrent[row] & (1 << index)) {// if the current physical button state is depressed
        _buttonEvents[row] = 1 << index;// queue up a new button event immediately
        _buttonStates[row] |= (1 << index);// and set the debounced state to down.
    }
      else{
        _buttonDebounceCounter[row][index] = _buttonDebounceTime;
      }  // otherwise the button was previously depressed and now
      // has been released so we set our debounce counter.
    }
    else if (((_buttonCurrent[row] ^ _buttonLast[row]) & (1 << index)) == 0 &&  // if the current physical button state is the same as
    (_buttonCurrent[row] ^ _buttonStates[row]) & (1 << index)) {        // the last physical button state but the current physical
      // button state is different from the current debounce 
      // state...
      if (_buttonDebounceCounter[row][index] > 0 && --_buttonDebounceCounter[row][index] == 0) {  // if the the debounce counter has
        // been decremented to 0 (meaning the button has been up for _buttonDebounceTime iterations///
        _buttonEvents[row] = 1 << index;    // queue up a button state change event
        if (_buttonCurrent[row] & (1 << index)){          // and toggle the buttons debounce state.
          _buttonStates[row] |= (1 << index);
        }
        else{
          _buttonStates[row] &= ~(1 << index);
        }
      }
    }
  }
  
  void SugarCube::initAnalogPins()
  {
    _xAccRaw = analogRead(_xAccPin);
    _yAccRaw = analogRead(_yAccPin);
    _xGyro = analogRead(_xGyroPin);
    _yGyro = analogRead(_yGyroPin);
    _pot1 = analogRead(_pot1Pin);
    _pot2 = analogRead(_pot2Pin);
  }
  
  void SugarCube::checkAnalogPins()
  {
    this->setXAcc(this->analogValFromPin(_xAccPin, _xAccRaw));
    this->setYAcc(this->analogValFromPin(_yAccPin, _yAccRaw));
    this->setXGyro(this->analogValFromPin(_xGyroPin, _xGyro));
    this->setYGyro(this->analogValFromPin(_yGyroPin, _yGyro));
    this->setPot1(this->analogValFromPin(_pot1Pin, _pot1));
    this->setPot2(this->analogValFromPin(_pot2Pin, _pot2));
    this->checkForShake();
  }
  
  int SugarCube::analogValFromPin(byte pinNum, int oldVal)
  {
    int newVal = analogRead(pinNum);
    if (abs(newVal-oldVal)>_analogTolerance){
      return newVal;
    }
    return oldVal;
  }
  
  void SugarCube::setXAcc(int newVal)
  {
    if (_xAccRaw != newVal){
      byte scaledNewVal = this->scaleAcc(newVal);
      if (this->scaleAcc(_xAccRaw) != scaledNewVal){
        _xAccRaw = newVal;
        _delegate->xAccHasChanged(scaledNewVal);
      }
    }
  }
  
  void SugarCube::setYAcc(int newVal)
  {
    if (_yAccRaw != newVal){
      byte scaledNewVal = this->scaleAcc(newVal);
      if (this->scaleAcc(_yAccRaw) != scaledNewVal){
        _yAccRaw = newVal;
        _delegate->yAccHasChanged(scaledNewVal);
      }
    }
  }
  
  byte SugarCube::scaleAcc(int rawVal)
  {
    //between 335 and 231
    return constrain(map(rawVal, 335, 231, 0, 127), 0, 127);
  }
  
  void SugarCube::setXGyro(int newVal)
  {
    if (_xGyro != newVal){
      _xGyro = newVal;
      _delegate->xGyroHasChanged(newVal);
    }
  }
  
  void SugarCube::setYGyro(int newVal)
  {
    if (_yGyro != newVal){
      _yGyro = newVal;
      _delegate->yGyroHasChanged(newVal);
    }
  }
  
  void SugarCube::setPot1(int newVal)
  {
    if (_pot1 != newVal){
      _pot1 = newVal;
      _delegate->pot1HasChanged(newVal);
    }
  }
  
  void SugarCube::setPot2(int newVal)
  {
    if (_pot2 != newVal){
      _pot2 = newVal;
      _delegate->pot2HasChanged(newVal);
    }
  }
  
  void SugarCube::checkForShake()
  {
    //acc raw values between 335 and 231
    if (_shakeDebounceTimer<=1000 && _shakeDebounceTimer>=0) {
      if (_numShakeExtremes == 0) _shakeDebounceTimer=0;
      if ((_xAccRaw<260 && _xAccMax) || (_xAccRaw>300 && !_xAccMax)){//if we hit a max
        _numShakeExtremes++;
        _xAccMax = !_xAccMax;
      }
      if ((_yAccRaw<260 && _yAccMax) || (_yAccRaw>300 && !_yAccMax)){//if we hit a max
        _numShakeExtremes++;
        _yAccMax = !_yAccMax;
      }
    }
    if (_numShakeExtremes>3){
      _delegate->wasShaken();
      _numShakeExtremes = 0;
      _shakeDebounceTimer=-1000;
    }
    if (_shakeDebounceTimer>1000) {
      _numShakeExtremes = 0;
      _shakeDebounceTimer=0;
    }
     _shakeDebounceTimer++;
  }
  
  //---------------------------------------------------------------------
  //-----------------------MIDI/SERIAL-----------------------------------
  //---------------------------------------------------------------------
  
  void SugarCube::setupMIDICommunication()
  {
    //disable timer 2 interrupt
    Serial.begin(9600);
//    Serial.begin(31250);//MIDI baud rate
  }
  
  void SugarCube::noteOn(byte note, byte velocity, byte channel)
  {
    this->sendMIDI(144|channel, note, velocity);
  }
  
  void SugarCube::noteOff(byte note, byte channel)
  {
    this->sendMIDI(144|channel, note, 0);//note on message with zero velocity == noteoff
  }
  
  void SugarCube::pitchBend(byte pitchbend, byte channel)
  {
    this->sendMIDI(224|channel, 0, pitchbend);//command&channel, lsb, msb
  }
  
  void SugarCube::sendMIDI(byte command, byte param1, byte param2) 
  {//sends a MIDI message
    Serial.print(command);
    Serial.print("   ");
    Serial.print(param1);
    Serial.print("   ");
    Serial.println(param2);
//    Serial.write(command);
//    Serial.write(param1);
//    Serial.write(param2);
  }
  
  void SugarCube::setupSerialCommunication()
  {
    Serial.begin(9600);
    this->timer2Setup();
  }
  
  //---------------------------------------------------------------------
  //--------------------INTERRUPT ROUTINES-------------------------------
  //---------------------------------------------------------------------
  
  void SugarCube::timer1Routine()//1kHz
  {
    this->shift(_hardwareIter);
    _hardwareIter++;
    if (_hardwareIter>3){
      _hardwareIter = 0;
    }
    this->checkAnalogPins();
    if (_delegateTimer++>10) {
      _delegate->routine100kHz();
      _delegateTimer = 0;
    }
  }
  
  void SugarCube::timer2Routine()
  {
    if (_serialEnabled){//if in serial mode
      do{
        if (Serial.available()){
          byte ledByte = Serial.read();//000xxyys
          boolean ledstate = ledByte & 1;
          byte ledy = (ledByte >> 1) & 3;
          byte ledx = (ledByte >> 3) & 3;
          if (ledstate){
            _ledData[ledy] |= 8 >> ledx;
          }
          else{
            _ledData[ledy] &= ~ (8 >> ledx);
          }
        }//end if serial available
      }//end do
     while (Serial.available() > 0);
    }
  }
  
  //---------------------------------------------------------------------
  //-----------------------------DELEGATE--------------------------------
  //---------------------------------------------------------------------
  
  void SugarCube::setDelegate(Delegate * newDelegate)
  {
    _delegate = newDelegate;
    _delegate->setSugarCube(this);
  }
  
  

