// CServoDC.h

#ifndef _CSERVODC_h
#define _CSERVODC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class CServoDCClass
{
 protected:


 public:
	void init();
};

extern CServoDCClass CServoDC;

#endif

