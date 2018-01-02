#include "DueTimer.h"
#define pinport PIOC
#define pinmask (1<<29)   // Arduino pin 2 is port B pin 25.



bool  pulseSet;

void setup(){
  pinMode(10, OUTPUT);

  Timer3.attachInterrupt(callback);
  Timer3.start(1); // Calls every 50ms
}

void loop(){

  while(1){
    // I'm stuck in here! help me...
  }
  
}
void callback()
{
  pulseSet=!pulseSet;
  if(pulseSet)
  pinport -> PIO_SODR = pinmask ;   // set pin
  else
  pinport -> PIO_CODR = pinmask ;  // clear pin
  //digitalWrite(10, digitalRead(10) ^ 1);
}
