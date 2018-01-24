#include <DueTimer.h>
bool a;
  // x2- 2500~8000
  // x8  11000~40000
  // x16  20000~60000
  #define MAX_SPEED 60000
  #define MIN_SPEED 10000
void myHandler(){
  a=!a;
  if(a)
  {
    REG_PIOC_CODR = B00000100;
    REG_PIOC_SODR = B00001000;
  }
  else
  {
    REG_PIOC_SODR = B00000100;
    REG_PIOC_CODR = B00001000;
  }
}
////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  stepperEnable(true);
  Timer3.attachInterrupt(myHandler);
  Serial.begin(115200);
}
void stepperEnable(bool en)
{
  if(en)
  {
    digitalWrite(30, LOW);
    digitalWrite(31, HIGH);
  }
  else
  {
    digitalWrite(30, HIGH);
    digitalWrite(31, LOW);
  }
}
int spd=20000;
int oldSpeed = 0;
void stepperSpeedSet(int sspeed)
{
  if(sspeed>0)
  {
    digitalWrite(32, LOW);
    digitalWrite(33, HIGH);
  }
  else
  {
    digitalWrite(32, HIGH);
    digitalWrite(33, LOW);
    sspeed=-sspeed;
  }
  if(sspeed>MAX_SPEED)sspeed=MAX_SPEED;
  int newspeed = oldSpeed+ (sspeed - oldSpeed)/3;
  if(newspeed<MIN_SPEED)newspeed = MIN_SPEED;
  Timer3.start(1000000/newspeed); // Calls every 50ms
  oldSpeed = newspeed;
}

void loop() {

  //spd+=500;
  stepperSpeedSet(spd);
  delay(5);
  //Serial.println(spd);
  
}
