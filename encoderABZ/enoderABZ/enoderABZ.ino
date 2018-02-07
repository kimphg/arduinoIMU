int val;
#define encoder0PinA 2//pe4
#define encoder0PinB 3//pe5
#define encoder0PinZ 5//pe3
int encoder0Pos = 0;
int aold = LOW;
int n ;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (encoder0PinZ, INPUT);
  Serial.begin (1000000);
}

void loop() {
  n = PORTE;
  int ain = (PORTE&B0010000);
  int bin = (n>>5)&0x01;
  int zin = (n>>3)&0x01;
 
  if((ain ) && (aold == 0))
  {
    if (bin == true) {
      encoder0Pos++;
    } else {
      encoder0Pos--;
    }
     Serial.write(encoder0Pos);
    
  }
  aold = ain;
  
  if(zin)
  {
    
    encoder0Pos = 0;
  }
}
