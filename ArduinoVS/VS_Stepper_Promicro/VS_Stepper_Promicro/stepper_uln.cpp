// 
// 
// 

#include "stepper_uln.h"

StepperMicro::StepperMicro(int i1,int i2,int i3,int i4)
{
	IN1 = i1;
	IN2 = i2;
	IN3 = i3;
	IN4 = i4;
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	Steps = 0;
}

void StepperMicro::UpdateDirection(){
	if (speed>0)Steps++;
	else Steps--; 
	if (Steps>7){ Steps = 0; }
	if (Steps<0){ Steps = 7; }
}
void StepperMicro::Update()
{
	currentMillis = micros();
	int absSpeed = speed>0?speed:-speed;
	if (absSpeed)
	if (currentMillis - last_time >= 256000 / (absSpeed + 1)){
		Step();
		//time = time + micros() - last_time;
		last_time = currentMillis;
	}
	//Serial.println(speed);
	speed += acc;
}
void StepperMicro::Step()
{
	switch (Steps){
	case 0:
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);
		break;
	case 1:
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, HIGH);
		break;
	case 2:
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);
		break;
	case 3:
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);
		break;
	case 4:
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
		break;
	case 5:
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, HIGH);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
		break;
	case 6:
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
		break;
	case 7:
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);
		break;
	default:
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, LOW);
		break;
	}
	UpdateDirection();
}
void StepperMicro::Step(int xw){
	for (int x = 0; x<xw; x++)
	{
		Step();
	}
}
StepperMicro::~StepperMicro()
{
}
