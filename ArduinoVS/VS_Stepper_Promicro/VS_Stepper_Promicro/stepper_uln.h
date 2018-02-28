// stepper_uln.h

#ifndef _STEPPER_ULN_h
#define _STEPPER_ULN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
/*
BYJ48 Stepper motor code
Connect :
IN1 >> D8
IN2 >> D9
IN3 >> D10
IN4 >> D11
VCC ... 5V Prefer to use external 5V Source
Gnd
written By :Mohannad Rawashdeh
https://www.instructables.com/member/Mohannad+Rawashdeh/
28/9/2013
*/
class StepperMicro
{
public:
	StepperMicro(int i1, int i2, int i3, int i4);
	~StepperMicro();
	void UpdateDirection();
	void Step(int xw);
	void Step();
	void Update();
	float acc , speed ;
private:
	unsigned long last_time;
	unsigned long currentMillis;
	int IN1, IN2, IN3, IN4;
	int Steps;
};


#endif

