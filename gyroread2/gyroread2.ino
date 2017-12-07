
//Arduino 1.0+ only
#define EDIT_BY_QT
#ifdef EDIT_BY_QT
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "libs/l3d.h"
#define GYRO_SUM 1
#define ACC_SUM 5
#include <Servo.h>
Servo myservo;
Adafruit_MMA8451 mma = Adafruit_MMA8451();
struct value_t
{
  int curr;
  int old;
  long int sum;
};
struct sensor_data_t
{
  float dx,dy,dz;
  float x,y,z;
}sensorData;
value_t gyrox,gyroy,gyroz,accx,accy,accz;
float  gyroxOffset=-22.86;
float  gyroyOffset=-65.33;
float  gyrozOffset=105.97;
int gyroSumCounter =0;
int accSumCounter =0;
void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();
  Serial.begin(500000);
  Serial1.begin(9600);
  setupL3G4200D(500); // Configure L3G4200  - 250, 500 or 2000 deg/sec
  delay(1500); //wait for the sensors to be ready
  //calibGyro();
  Serial.println("starting up L3G4200D");
  digitalWrite(LED_BUILTIN, LOW);
  if (! mma.begin(0x1C)) {
    Serial.println("Could not start MMA");
    //while (1);
  }
  else Serial.println("MMA8451 found!");

  mma.setRange(MMA8451_RANGE_2_G);
<<<<<<< HEAD
  mma.setDataRate(MMA8451_DATARATE_100_HZ);//800hz
  myservo.attach(9);
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  digitalWrite(8,HIGH);
=======
  mma.setDataRate(MMA8451_DATARATE_800_HZ);//800hz
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
>>>>>>> origin/master
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
      TCCR5B |= 1<<CS50;
      //TCCR1B |= 1<<CS11;  
      // enable timer compare interrupt
      TIMSK5 |= (1 << OCIE5A);
  }

byte datagram[]={0xAA,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x00,0x00,0x00,0x00,0x00,0xCC};
<<<<<<< HEAD
float cP =0.0004,cD = 0.005,cI = 0.000001; 
float pitch =0;
float angle = 0;
void loop(){
   readSensors();
   //mma stabilization
   if(mma.y==0)return;
  pitch +=(atan(mma.x/(float)mma.y));
  
  angle+= gyroz.old*cI +(gyroz.curr)*cP+(gyroz.curr - gyroz.old)*cD+pitch/1000.0;
  while(angle>180) angle = 180;
  while(angle<0)angle = 0;
  myservo.write(angle);
 // yControl+=cP*(pitch)-gyroy.curr/3754.9;//3754.9 = 32768.0*500/57.3;
  //tone(9,65535);
  Serial.println(gyroz.curr);
  //Serial1.print(yControl);
=======
float cP =1,cD = 1,cI = 1; 
typedef union 
{
  float value;
  byte binary[4];
  }binary_float;
binary_float yControl;
int motorSpeed = 100;
void loop(){
   readSensors();
   float pitch = mma.x/(float)mma.z;
  //motorSpeed = 3000*(pitch);//-gyroy.curr/65.536*57.296;//65.536= 32768.0/500;
  if(abs(motorSpeed)>255)
    {
      if(motorSpeed>0)motorSpeed = 255;
      else motorSpeed = -255;
      }
  OCR1A =  (256-abs(motorSpeed))*100;
  
  if(motorSpeed>0)
  {
    digitalWrite(11,LOW);
  }
  else
  {
    digitalWrite(11,HIGH);
    }
//  
  //Serial.println(yControl);
  //Serial1.println(yControl);
>>>>>>> origin/master
   

}
ISR(TIMER5_COMPA_vect){//timer1 interrupt  toggles pin 13 (LED)
  
  PORTB |= B00000010;
  delayMicroseconds(3);//5ms = 2.5 sec/turn
  
  if(motorSpeed)PORTB &= B11111101;
  
  return;

}
void readSensors()
{

    mma.read();
    
    //read gyro
    //gyrox.old = gyrox.curr;
    //gyroy.old = gyroy.curr;
    gyroz.old = gyroz.curr;
    getGyroValues();
    /*gyrox.sum += minAbs(gyrox.old , gyrox.curr);
    gyroy.sum += minAbs(gyroy.old , gyroy.curr);
    gyroz.sum += minAbs(gyroz.old , gyroz.curr);
    gyroSumCounter++;
    if (gyroSumCounter>=GYRO_SUM)
    {
        sensorData.dx = gyrox.sum/(double)gyroSumCounter;
        sensorData.dy = gyroy.sum/(double)gyroSumCounter;
        sensorData.dz = gyroz.sum/(double)gyroSumCounter;
        int xGyro = sensorData.dx;
        int yGyro = sensorData.dy;
        int zGyro = sensorData.dz;
        datagram[1]=xGyro>>8;
        datagram[2]=xGyro;
        datagram[3]=yGyro>>8;
        datagram[4]=yGyro;
        datagram[5]=zGyro>>8;
        datagram[6]=zGyro;
        Serial.write(datagram,14);
        //
        gyroSumCounter = 0;
        gyrox.sum = 0;
        gyroy.sum = 0;
        gyroz.sum = 0;
        
    }*/

}
inline int minAbs(int a,int b)
{
  
  return (abs(a)<abs(b))?a:b;
}
#define CALIB_MAX_COUNT 300.0
void calibGyro()
{
    gyroxOffset = 0;
    gyroyOffset = 0;
    gyrozOffset = 0;
    int calibCount =0;
    // calib mean value
    gyrox.sum = 0;
    gyroy.sum = 0;
    gyroz.sum = 0;
    while(calibCount<CALIB_MAX_COUNT)
    {
        delay(1);
        getGyroValues();  // This will update x, y, and z with new values
        gyrox.sum+=gyrox.curr;
        gyroy.sum+=gyroy.curr;
        gyroz.sum+=gyroz.curr;
        calibCount++;
    }
    calibCount =0;
    gyroxOffset += gyrox.sum/CALIB_MAX_COUNT;
    gyroyOffset += gyroy.sum/CALIB_MAX_COUNT;
    gyrozOffset += gyroz.sum/CALIB_MAX_COUNT;
    Serial.print("xMean:");
    Serial.println(gyroxOffset);
    Serial.print("yMean:");
    Serial.println(gyroyOffset);
    Serial.print("zMean:");
    Serial.println(gyrozOffset);
}

void getGyroValues(){

  byte xMSB = readRegister(L3G4200D_ADDRESS, 0x29);
  byte xLSB = readRegister(L3G4200D_ADDRESS, 0x28);

  gyrox.curr = (((xMSB << 8) | xLSB)-gyroxOffset);

  byte yMSB = readRegister(L3G4200D_ADDRESS, 0x2B);
  byte yLSB = readRegister(L3G4200D_ADDRESS, 0x2A);
  gyroy.curr = (((yMSB << 8) | yLSB)-gyroyOffset);

  byte zMSB = readRegister(L3G4200D_ADDRESS, 0x2D);
  byte zLSB = readRegister(L3G4200D_ADDRESS, 0x2C);
  gyroz.curr = (((zMSB << 8) | zLSB)-gyrozOffset);
}
#endif
