//Arduino 1.0+ only

#include <Wire.h>
#include "mma8451.h"
#include "l3d.h"
#define NUM_MES 10
//todo: show led
//toto measure acceleration

int xCur=0,xOld;
long int sumx = 0,sumMesX=0;
int yCur=0,yOld;
long int sumy = 0,sumMesY=0;
int zCur=0,zOld;
long int sumz = 0,sumMesZ=0;
double xVar, xMean=0;
double yVar, yMean=0;
double zVar, zMean=0;
int mesCount =0;
void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.begin();
  Serial.begin(500000);
  setupL3G4200D(500); // Configure L3G4200  - 250, 500 or 2000 deg/sec
  delay(1500); //wait for the sensor to be ready 
  calib1();
  calib2();
  Serial.println("starting up L3G4200D");
  digitalWrite(LED_BUILTIN, LOW);
}
byte datagram[]={0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0xCC};
void loop(){
   xOld = xCur;
   yOld = yCur;
   zOld = zCur;
   getGyroValues();  // This will update x, y, and z with new values
   int xMin = minAbs(xOld , xCur);
   int yMin = minAbs(yOld , yCur);
   int zMin = minAbs(zOld , zCur);
   sumx+=xMin;
   sumy+=yMin;
   sumz+=zMin;
  if (mesCount>=NUM_MES)
  {
    unsigned int mesX = sumx/NUM_MES;
    unsigned int mesY = sumy/NUM_MES;
    unsigned int mesZ = sumz/NUM_MES;
    
    datagram[1]=mesX>>8;
    datagram[2]=mesX;
    datagram[3]=mesY>>8;
    datagram[4]=mesY;
    datagram[5]=mesZ>>8;
    datagram[6]=mesZ;
    //Serial.write(datagram,8);
    //
    mesCount = 0;
    sumx = 0;
    sumy = 0;
    sumz = 0;
    readMMA8451();
  }
    
}
inline int minAbs(int a,int b)
{
  if(abs(a)<abs(b))return a;
  else return b;
}
#define CALIB_MAX_COUNT 300.0
void calib1()
{
  int calibCount =0;
  // calib mean value
  sumx = 0;
  sumy = 0;
  sumz = 0;
  while(calibCount<CALIB_MAX_COUNT)
  {
    delay(1);
   getGyroValues();  // This will update x, y, and z with new values
   sumx+=xCur;
   sumy+=yCur;
   sumz+=zCur;
    calibCount++;
  }
  calibCount =0;
  xMean += sumx/CALIB_MAX_COUNT;
  yMean += sumy/CALIB_MAX_COUNT;
  zMean += sumz/CALIB_MAX_COUNT;
  int sumXVar=0;
  int sumYVar=0;
  int sumZVar=0;
  while(calibCount<CALIB_MAX_COUNT)
  {
    delay(1);
    getGyroValues();
    sumXVar+=abs(xCur);
    sumYVar+=abs(yCur);
    sumZVar+=abs(zCur);
    calibCount++;
  }
  xVar = sumXVar/CALIB_MAX_COUNT;
  yVar = sumYVar/CALIB_MAX_COUNT;
  zVar = sumZVar/CALIB_MAX_COUNT;
  }
void calib2()
{
  int calibCount =0;
  // calib mean value
  sumx = 0;
  sumy = 0;
  sumz = 0;
  while(calibCount<CALIB_MAX_COUNT)
  {
    delay(1);
   xOld = xCur;
   yOld = yCur;
   zOld = zCur;
   getGyroValues();  // This will update x, y, and z with new values
   int xMin = minAbs(xOld , xCur);
   int yMin = minAbs(yOld , xCur);
   int zMin = minAbs(zOld , xCur);
   sumx+=xMin;
   sumy+=yMin;
   sumz+=zMin;
    calibCount++;
  }
  calibCount =0;
  xMean += sumx/CALIB_MAX_COUNT;
  yMean += sumy/CALIB_MAX_COUNT;
  zMean += sumz/CALIB_MAX_COUNT;
  
  int sumXVar=0;
  int sumYVar=0;
  int sumZVar=0;
  while(calibCount<CALIB_MAX_COUNT)
  {
    delay(1);
    getGyroValues();
    sumXVar+=abs(xCur);
    sumYVar+=abs(yCur);
    sumZVar+=abs(zCur);
    calibCount++;
  }
  xVar = sumXVar/CALIB_MAX_COUNT;
  yVar = sumYVar/CALIB_MAX_COUNT;
  zVar = sumZVar/CALIB_MAX_COUNT;
  Serial.println("calib done:");
  Serial.print("xVar:");
  Serial.println(xVar);
  Serial.print("yVar:");
  Serial.println(yVar);
  Serial.print("zVar:");
  Serial.println(zVar);
  Serial.print("xMean:");
  Serial.println(xMean);
  Serial.print("yMean:");
  Serial.println(yMean);
  Serial.print("zMean:");
  Serial.println(zMean);
  }
void getGyroValues(){

  byte xMSB = readRegister(L3G4200D_ADDRESS, 0x29);
  byte xLSB = readRegister(L3G4200D_ADDRESS, 0x28);
  
  xCur = (((xMSB << 8) | xLSB)-xMean+0.5);

  byte yMSB = readRegister(L3G4200D_ADDRESS, 0x2B);
  byte yLSB = readRegister(L3G4200D_ADDRESS, 0x2A);
  yCur = (((yMSB << 8) | yLSB)-yMean+0.5);

  byte zMSB = readRegister(L3G4200D_ADDRESS, 0x2D);
  byte zLSB = readRegister(L3G4200D_ADDRESS, 0x2C);
  zCur = (((zMSB << 8) | zLSB)+0.5-zMean);
  mesCount +=1;
}
void readMMA8451(void) {
  // read x y z at once
  Wire.beginTransmission(MMA8451_DEFAULT_ADDRESS);
  i2cwrite(MMA8451_REG_OUT_X_MSB);
  Wire.endTransmission(false); // MMA8451 + friends uses repeated start!!

  Wire.requestFrom(MMA8451_DEFAULT_ADDRESS, 6);
  int x = Wire.read(); x <<= 8; x |= Wire.read(); x >>= 2;
  int y = Wire.read(); y <<= 8; y |= Wire.read(); y >>= 2;
  int z = Wire.read(); z <<= 8; z |= Wire.read(); z >>= 2;

  
//  uint8_t range = getRange();
  uint16_t divider = 2048;
//  if (range == MMA8451_RANGE_8_G) divider = 1024;
//  if (range == MMA8451_RANGE_4_G) divider = 2048;
//  if (range == MMA8451_RANGE_2_G) divider = 4096;

  float x_g = (float)x ;// divider;
  float y_g = (float)y ;// divider;
  float z_g = (float)z ;// divider;
  
  Serial.print("x_g:");
  Serial.println(x_g);
  Serial.print("y_g:");
  Serial.println(y_g);
  Serial.print("z_g:");
  Serial.println(z_g);
}

