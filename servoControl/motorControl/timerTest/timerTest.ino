#include "timerThree.h"
#define BAUD_SERIAL 1000000
void setup()
{
  Serial.begin(BAUD_SERIAL);
   pinMode(10, OUTPUT);
  Timer3.initialize(1);         // initialize timer1, and set a 1/2 second period
  Timer3.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer3.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
bool pulseSet;
void loop()
{
  
  while(Serial.available())
  {
    int value = Serial.read();
    Timer3.setPeriod(value);
    Serial.println(value);
  }
}
void callback()
{
  pulseSet^=1;
  if(pulseSet)
  PORTB = B00010000;

  else PORTB = B00000000;
  //digitalWrite(10, digitalRead(10) ^ 1);
}
