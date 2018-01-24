#define TICH_LUY    256
#define CLOCK_PIN   10
#define DATA_PIN    11
#define BIT_COUNT   10
#define CHIP_SEL    9
unsigned char buf[5];
void setup() {
  pinMode(DATA_PIN, INPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(CHIP_SEL, OUTPUT);
  digitalWrite(CLOCK_PIN, HIGH);
  digitalWrite(CHIP_SEL, HIGH);
  Serial.begin(9600);
   buf[0]=0xff;
}
int data[TICH_LUY];
int j = 0;
long oldSum=0;
void loop() {
  data[j] = readPosition();
  j++;
  if(j>=TICH_LUY)
  {
    long sum =0;
    //int maxData=0;
    //int minData=1023;
    for(int i=0;i<TICH_LUY;i++)
    {
      int dataShift = data[i]-data[0];
      sum+=data[i];
      if(dataShift>511)sum-=1024;
      else 
      if(dataShift<-511)sum+=1024;      
    }
    if(sum>262144)sum-=262144;
    if(sum<0)sum+=262144;
    /*
    long diff=sum-oldSum;
    if(diff>131072)
    {
      oldSum+=262144;
      sum=(sum+oldSum)/2;
      if(sum>=262144)sum-=262144;
    }
    else if(diff<-131072)
    {
      oldSum-=262144;
      sum=(sum+oldSum)/2;
      if(sum<0)sum+=262144;
    }
    else
    {
      sum=oldSum+(sum-oldSum)/5;
    }
    oldSum = sum;
    // double output;
    //if(maxData-minData>100)output = 0;//minData<<8;
    //else 
    //output = sum;
    */
    buf[1] = (sum>>16);//&0x7f;
    buf[2] = sum>>8;//&0xff;
    buf[3] = sum;
    Serial.write(buf,4);
    /*
    //Serial.println(0);
    Serial.println(output);
    //Serial.println(minData);
    //Serial.println(maxData);
    */
    j=0;  
  }

}

//read the current angular position
int readPosition() {
  digitalWrite(CHIP_SEL, LOW);
  unsigned int data = 0;
  for (int i=0; i<BIT_COUNT; i++) {
    data <<= 1;
    digitalWrite(CLOCK_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(CLOCK_PIN, HIGH);
    delayMicroseconds(1);
    data |= digitalRead(DATA_PIN);
  }
  digitalWrite(CHIP_SEL, HIGH);
  return data;
}


