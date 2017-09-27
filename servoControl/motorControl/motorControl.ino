void setup() {
    DDRB = B00111111;
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    //int timeValue = analogRead(A0);
    cli();//stop interrupts
    timer1Period(100);//1= 2 microsec
    sei();//allow interrupts
  

}
void timer1Period(int twoMicro)
{
  TCCR1A = 0;// set entire TCCR1A register to 0
      TCCR1B = 0;// same for TCCR1B
      TCNT1  = 0;//initialize counter value to 0
      // set compare match register for 1hz increments
      OCR1A =  twoMicro;//(16000000) / (hz*8) - 1 ;//(must be <65536)
      // turn on CTC mode
      TCCR1B |= (1 << WGM12);
      // Set CS10 and CS12 bits for 1024 prescaler
      TCCR1B |= 1<<CS10;
      //TCCR1B |= 1<<CS11;  
      // enable timer compare interrupt
      TIMSK1 |= (1 << OCIE1A);
  }
void loop() {
  // put your main code here, to run repeatedly:
  
}
bool toggle1;
int count = 0;
void delayClock()
{}
ISR(TIMER1_COMPA_vect){//timer1 interrupt  toggles pin 13 (LED)
  
  PORTB |= B00000010;
  delayMicroseconds(3);//5ms = 2.5 sec/turn
  PORTB &= B11111101;
  
  return;
//  count++;
//  if(count<2)return;
//  count =0;
//  if(toggle1)
//  {
//    toggle1 = false;
//    PORTB |= B00000010;
//    //digitalWrite(9,HIGH);
//  }
//  else
//  {
//    toggle1 = true;
//    PORTB &= B11111101;
//    //digitalWrite(9,LOW);//PORTB = B1101;//|= B000010;
//  }return;
}
  

