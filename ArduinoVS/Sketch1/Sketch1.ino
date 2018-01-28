#include "CServoDC.h"
#include <DueTimer.h>
#define InA 12
#define InB 13
#define EA 10
#define  EB 11
#define ROT 768
int controlP = 13;
int controlD = 60;
int Buzzer_pin = 4;
int dspeed = 10;
int newpos = 50;
#define SPEED_MAX 254
#define SPEED_MIN -254
void setup() {
	// put your setup code here, to run once:
	pinMode(Buzzer_pin, OUTPUT);
	digitalWrite(Buzzer_pin, 1);
	delay(100);
	pinMode(InA, OUTPUT);
	pinMode(InB, OUTPUT);
	pinMode(EA, OUTPUT);
	pinMode(EB, OUTPUT);
	pinMode(6, INPUT);
	pinMode(7, INPUT);
	//set speed
	setSpeedA(0);
	Serial.begin(115200);
	Timer2.attachInterrupt(readEncoder);
	Timer2.start(20); // Calls every 20us
	Timer3.attachInterrupt(readSpeed);
	Timer3.start(2000); // Calls every 2ms
	digitalWrite(Buzzer_pin, 0);

}


void setSpeedA(int ispeed)// from -255 to 255
{
	if (ispeed < 0)
	{
		int speed = -ispeed;
		digitalWrite(InA, LOW);
		analogWrite(EA, uint(speed));
	}
	else
	{
		int speed = ispeed;
		digitalWrite(InA, HIGH);
		analogWrite(EA, uint(speed));
	}
}
bool oldPinA;
int encoderVal = 0;

void readEncoder()
{
	bool newpinA = digitalRead(6);
	if (newpinA&&(!oldPinA))
	{
		if (digitalRead(7))
			encoderVal++;
		else
			encoderVal--;
		//Serial.println(encoderVal);
	}
	//else Serial.write('.');
	oldPinA = newpinA;
}
int oldEncoderVal = 0;
int realSpeed = 0;
int oldSpeed;
void readSpeed()
{
	realSpeed = encoderVal - oldEncoderVal;
	oldEncoderVal = encoderVal;
	int newspeed = (newpos - encoderVal) * controlP - realSpeed*controlD;
	if (newspeed > 255)newspeed = 255;
	if (newspeed < -255)newspeed = -255;
	if (oldSpeed != newspeed)
	{
		setSpeedA(newspeed);
		oldSpeed = newspeed;
		
		//if (encoderVal > newpos + 1)Serial.println(encoderVal);
	}
	Serial.println(realSpeed);
}
String inString = "";
void readCommand()
{
	while (Serial.available() > 0) {
		int inChar = Serial.read();
		if (isDigit(inChar)) {
			// convert the incoming byte to a char and add it to the string:
			inString += (char)inChar;
		}
		// if you get a newline, print the string, then the string's value:
		if (inChar == '\n') {
			newpos = inString.toInt();
			Serial.println(controlD);
			inString = "";
		}
	}
}
int velo = -10;
void loop() {
	
		
	if (Serial.available())
	{
		readCommand();
		
	}
	//delay(5000);
	//newpos += 500;
	
	
}