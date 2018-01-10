
/* Arduino Port Manipulation to Generate Clock Signal
* 02/18/2012  
*/

/*Please see the "Port Manipulation" article here: http://www.arduino.cc/playground/Learning/PortManipulation
   *NOTE: The ATmega328 (used on the UNO) has an identical pin configuration to the ATmega168 (described in the Port Manipulation documentation). http://arduino.cc/en/Main/arduinoBoardUno
   *ATmega328 has three ports: D - Digital Pins 0-7, B - Digital Pins 8-13, C - Analog Pins 0-5
   *For safety, do not use:
   *Port D - Pins 0 & 1 are used for serial communications for programming and bebugging the Arduino.  This can mess up the chip's send and receive.
   *
   *DDR is the direction register and sets whether the port is input or output
   *To set input/output use DDR<LETTER> - 0 is input, 1 is output
   *Alternatively, since it is only run once during setup(), it is satisfactory to use pinMode(pinNumber, OUTPUT);
   *
   *PORT is the register for the state of the outputs.  It sets whether the value of an output pin is high (1) or low (0) 
   *To set state of a specific pin on the Arduino UNO, use PORT<LETTER><PIN_NUMBER>
   *
   *PIN is the input register variable.  It will read all of the digital input pins at the same time
   *To read the pins on a specific port use PIN<Letter>
   *   
*/ 

//int DATA_MINUS_PIN = 2; //Pin for reading minus signal
int DATA_PLUS_PIN = 12; //Pin for reading plus signal

//int CLOCK_MINUS_PIN = 11; //Pin for clock minus signal
int CLOCK_PLUS_PIN = 11; //Pin for clock plus signal

void setup()
{ 
   //pinMode(CLOCK_MINUS_PIN, OUTPUT); //Set clock minus pin as output
   pinMode(CLOCK_PLUS_PIN, OUTPUT); //Set clock plus pin as output
   
   //pinMode(DATA_MINUS_PIN, INPUT); //Set data minus pin as input
   pinMode(DATA_PLUS_PIN, INPUT); //Set data plus pin as input
   
   delay(500);
   Serial.begin(115200); //Set serial baud rate
}

void loop()
{
  long int val = 0;
  for(int i=0;i<=19;i++)
  {
    digitalWrite(CLOCK_PLUS_PIN,HIGH); //Set Port B pins 11,12 HIGH, others LOW
    //Serial.println(PINB>>5); //Read all Port D pins (0-7) simultaneously and print
    if(i>0)
    {val+=(digitalRead(DATA_PLUS_PIN));
    val = val<<1;}
    delayMicroseconds(10); //Delay for 10 microseconds for timing
    digitalWrite(CLOCK_PLUS_PIN,LOW);
    delayMicroseconds(10); //Delay for 10 microseconds for timing
    
    //Serial.println(PINB); //Read all Port D pins (0-7) simultaneously and print
  }
  Serial.println(val>>3);
  delay(100);
}
