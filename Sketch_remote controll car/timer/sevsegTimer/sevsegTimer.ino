#include "SevSeg.h"
#include "TimerOne.h"
SevSeg sevseg; //Instantiate a seven segment controller object

static unsigned long timer=0;
static int deciSeconds=0;
static int Min=0; 
unsigned long time_start=0;
unsigned long time_end=0;

void setup() {
  byte numDigits=4;
  byte digitPins[]={2, 3, 4, 5};
  byte segmentPins[]={6, 7, 8, 9, 10, 11, 12, 13};
  byte hardwareConfig=COMMON_CATHODE;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(100);

  Timer1.initialize(100000); //initiallize timer routine 100000micros=0.1s
  Timer1.attachInterrupt(timerIsr); //set timer as sexagesimal system
  Serial.begin(115200);
}

void loop() {
  //timer start
  if(analogRead(A1) && time_start==0){
    time_start=millis();
  }
  if(analogRead(A2) && time_start>0){
    time_end=millis();
  }
  if(analogRead(A1) && time_start>0){
    if ((millis()-time_start)-timer>=100){
      timer+=100;
      deciSeconds++;
      sevseg.setNumber(Min*1000+deciSeconds, 1); //set (number, decimal point)
    }
    sevseg.refreshDisplay(); //Run repeatedly
  }
  //timer stop
  while(analogRead(A2)){
    sevseg.setNumber(Min*1000+deciSeconds, 1);    
    sevseg.refreshDisplay();
    if(analogRead(A2) && millis()-time_end>=5000){ //wait for 5sec rste timer
      timer=0;
      deciSeconds=0;
      Min=0;
      time_start=0;
      time_end=0;
    }
  }
}

void timerIsr(){
  if(deciSeconds>=599){
    deciSeconds = 0;
    Min++;
    if(Min>9){
      Min = 0;  
    }
  }
}
