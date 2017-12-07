#include <SoftwareSerial.h>
SoftwareSerial myserial(2,3);
void setup() {
  Serial.begin(115200);
  myserial.begin(9600);
    DDRB = B00111111;
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);
    
    //int timeValue = analogRead(A0);
    cli();//stop interrupts
    timer1Period(25500);//1= 2 microsec
    sei();//allow interrupts
}
void timer1Period(int twoMicro)
{
  TCCR5A = 0;// set entire TCCR1A register to 0
      TCCR5B = 0;// same for TCCR1B
      TCNT5  = 0;//initialize counter value to 0
      // set compare match register for 1hz increments
      OCR5A =  twoMicro;//(16000000) / (hz*8) - 1 ;//(must be <65536)
      // turn on CTC mode
      TCCR5B |= (1 << WGM52);
      // Set CS10 and CS12 bits for 1024 prescaler
      TCCR1B |= 1<<CS10;
      //TCCR1B |= 1<<CS11;  
      // enable timer compare interrupt
      TIMSK1 |= (1 << OCIE1A);
  }
int motorSpeed = -200;
byte dataFrame[14];
typedef union 
{
  float value;
  byte binary[4];
  }binary_float;
binary_float value ;
void loop() {
  // put your main code here, to run repeatedly:
//  if(motorSpeed<200)motorSpeed+=10;else motorSpeed=-200;
//  delay(500);

  //digitalWrite(11,HIGH);
  if(myserial.available())myserial.readBytes(dataFrame,1); else return;
  
  if(dataFrame[0]==0xAA)
  {
    dataFrame[0] = 0;
    myserial.readBytes(&dataFrame[1],13);
    value.binary[0] = dataFrame[1];
    value.binary[1] = dataFrame[2];
    value.binary[2] = dataFrame[3];
    value.binary[3] = dataFrame[4];
    digitalWrite(13,HIGH);
    motorSpeed = value.value*3000;
    if(abs(motorSpeed)>255)
    {
      if(motorSpeed>0)motorSpeed = 255;
      else motorSpeed = -255;
      }
    OCR1A =  (256-abs(motorSpeed))*100;
  }
  else
  {
    //digitalWrite(13,LOW);
    //motorSpeed = 0;
    
  }
  
  if(motorSpeed>0)
  {
    digitalWrite(11,LOW);
  }
  else
  {
    digitalWrite(11,HIGH);
    }
//  if(motorSpeed<255)motorSpeed+=50;else motorSpeed=0;
//  delay(2000);
  
//  OCR1A =  (256-abs(motorSpeed))*100;
}

ISR(TIMER5_COMPA_vect){//timer1 interrupt  toggles pin 13 (LED)
  
  PORTB |= B00000010;
  delayMicroseconds(3);//5ms = 2.5 sec/turn
  
  if(motorSpeed)PORTB &= B11111101;
  
  return;

}
  

