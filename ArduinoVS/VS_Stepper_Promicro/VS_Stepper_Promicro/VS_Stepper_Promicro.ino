/*
 Name:		VS_Stepper_Promicro.ino
 Created:	2/11/2018 10:53:33 AM
 Author:	Phuong
*/

// the setup function runs once when you press reset or power the board
#include "stepper_uln.h"

long time;
bool Direction;

StepperMicro stepper(5,6,7,8);
void setup()
{
	Serial.begin(115200);
	stepper.acc = 0.1;

}
void loop()
{
	if (stepper.speed >= 255)
	{
		stepper.acc = -0.005;
		Serial.println(stepper.speed);
	}
	if (stepper.speed <= -255)
	{
		stepper.acc = 0.005;
	}
	stepper.Update();
	
}
