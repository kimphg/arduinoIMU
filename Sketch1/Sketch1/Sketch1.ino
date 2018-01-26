int InA = 12;
int InB = 13;
int EA = 10;
int EB = 11;
int Buzzer_pin = 4;
void setup() {
	// put your setup code here, to run once:
	pinMode(InA, OUTPUT);
	pinMode(InB, OUTPUT);
	pinMode(EA, OUTPUT);
	pinMode(EB, OUTPUT);
	pinMode(Buzzer_pin, OUTPUT);

	digitalWrite(Buzzer_pin, 1);  // buzzer active HIGH
	delay(500);
	digitalWrite(Buzzer_pin, 0);
	delay(500);

	//set speed
	analogWrite(EB, 10);
	analogWrite(EA, 10);

}

void loop() {
	// put your main code here, to run repeatedly:

	digitalWrite(InA, HIGH);
	digitalWrite(InB, HIGH);
	delay(3000);
	digitalWrite(InA, LOW);
	digitalWrite(InB, LOW);
	delay(3000);
}