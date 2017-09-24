//Arduino 1.0+ only

#include <Wire.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define NUM_MES 3
int L3G4200D_Address = 105; //I2C address of the L3G4200D
int setupL3G4200D(int scale);
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

  Wire.begin();
  Serial.begin(500000);
  setupL3G4200D(500); // Configure L3G4200  - 250, 500 or 2000 deg/sec
  delay(1500); //wait for the sensor to be ready 
  //calib1();
  //calib2();
  Serial.println("starting up L3G4200D");
}
byte datagram[]={0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0xCC};
void loop(){
   xOld = xCur;
   yOld = yCur;
   zOld = zCur;
   getGyroValues();  // This will update x, y, and z with new values
   int xMin = xCur;//minAbs(xOld , xCur);
   int yMin = yCur;//minAbs(yOld , yCur);
   int zMin = zCur;//minAbs(zOld , zCur);
   sumx+=xMin;
   sumy+=yMin;
   sumz+=zMin;
  if (mesCount>=NUM_MES)
  {
    unsigned int mesX = sumx/NUM_MES;
    unsigned int mesY = sumy/NUM_MES;
    unsigned int mesZ = sumz/NUM_MES;
    xMean+=mesX/2000.0;
    yMean+=mesY/2000.0;
    zMean+=mesZ/2000.0;
    datagram[1]=mesX>>8;
    datagram[2]=mesX;
    datagram[3]=mesY>>8;
    datagram[4]=mesY;
    datagram[5]=mesZ>>8;
    datagram[6]=mesZ;
    Serial.write(datagram,8);
    //
    mesCount = 0;
    sumx = 0;
    sumy = 0;
    sumz = 0;
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

  byte xMSB = readRegister(L3G4200D_Address, 0x29);
  byte xLSB = readRegister(L3G4200D_Address, 0x28);
  
  xCur = (((xMSB << 8) | xLSB)-xMean+0.5);

  byte yMSB = readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = readRegister(L3G4200D_Address, 0x2A);
  yCur = (((yMSB << 8) | yLSB)-yMean+0.5);

  byte zMSB = readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = readRegister(L3G4200D_Address, 0x2C);
  zCur = (((zMSB << 8) | zLSB)+0.5-zMean);
  mesCount +=1;
}

int setupL3G4200D(int scale){
  //From  Jim Lindblom of Sparkfun's code

  // Enable x, y, z and turn off power down:
  writeRegister(L3G4200D_Address, CTRL_REG1, 0b01001111);//ODR=10(400), BW=11,PD =1, xyz = 111

  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  writeRegister(L3G4200D_Address, CTRL_REG2, 0b00001000);// HPF 0.1hz

  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

  // CTRL_REG4 controls the full-scale range, among other things:

  if(scale == 250){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
  }else{
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
  }

  // CTRL_REG5 controls high-pass filtering of outputs, use it
  // if you'd like:
  writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void writeRegister(int deviceAddress, byte address, byte val) {
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

    int v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();

    Wire.requestFrom(deviceAddress, 1); // read a byte

    while(!Wire.available()) {
        // waiting
    }

    v = Wire.read();
    return v;
}
