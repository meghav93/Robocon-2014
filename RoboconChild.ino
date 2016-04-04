#include <Stepper.h>
#include "Declarations.h"
#define RED true

const bool THEME = RED;

Fan fan(7);
stepper stp(400, 34, 32, 30, 28, 14);
Electromagnet em[3] = {2, 4, 6};
Encoder en[3] = { 52, 53, 54};
IR ir[3] = {47, 48, 49};

void setup()
{
	//stp.setSpeed(60);
	Serial.begin(9600);

	pinMode(22, OUTPUT);
	pinMode(24, OUTPUT);
	pinMode(26, OUTPUT);

	// attachInterrupt(A0,interrupt1,RISING);
	digitalWrite(22, LOW);
	digitalWrite(24, HIGH);
	digitalWrite(26, HIGH);
	stp.setSpeed(30);
	Serial.println("Ready");
	//  noInterrupts();
	//stp.step(400);
	//delay(1000);
	//stp.step(-400);
}

int lastAngle = 0;

void endRot()
{
	lastAngle += 12;
	if (lastAngle < 120) return;
	detachInterrupt(en[1].Pin);
	fan.stop();
}

void pole4()
{
	fan.stop();
	em[1].magnetize();
	delay(100);
	em[3].demagnetize();	
	stp.rotateImmediate(THEME == RED ? 120 : -120);
	fan.setSpeed(FanSpeed);
	attachInterrupt(en[1].Pin,endRot,RISING);
	lastAngle = 0;
}

void intE3()
{
	detachInterrupt(en[3].Pin);
	pole4();
}

void intIR1()
{
	detachInterrupt(ir[1].Pin);
	attachInterrupt(en[3].Pin,intE3,RISING);
}

void pole3()
{
	fan.stop();
	em[3].magnetize();
	delay(100);
	em[2].demagnetize();
	stp.rotateImmediate(THEME == RED ? -120 : 120);
	attachInterrupt(ir[1].Pin, intIR1, RISING);
}

void intE2()
{
	detachInterrupt(en[2].Pin);
	pole3();
}

void intIR3()
{
	detachInterrupt(ir[3].Pin);
	attachInterrupt(en[2].Pin,intE2,RISING);
}

void pole2()
{
	fan.stop();
	em[2].magnetize();
	delay(100);
	em[1].demagnetize();
	stp.rotateImmediate(THEME == RED ? 240:-240);
	//attachInterrupt(ir[3].Pin, intIR3, RISING);
	//fan.setSpeed(FanSpeed);
}

void intE1()
{
	detachInterrupt(en[1].Pin);
	pole2();
}

void intIR2()
{
	 attachInterrupt(en[1].Pin,intE1,RISING);
	 detachInterrupt(ir[2].Pin);
}

void pole1Start()
{
	attachInterrupt(ir[2].Pin,intIR2, RISING);
	fan.setSpeed(FanSpeed);
}


void pole1Init()
{
	em[1].magnetize();
	stp.initialize();
}

int cH,last=99;
long speed = 1170;
int totalSteps = 200;
void loop()
{
	fan.update();
	stp.update();


	if (Serial.available() > 0)
	{
		cH = Serial.read();
		Serial.flush();
		Serial.println((char)cH);

		switch (cH)
		{
			 
		//case 'q':
		//	e1.magnetize();
		//	break;
		//case '1':
		//	stp.initialize();
		//	//stp.rotateBy(90);
		//	break;
		//case 'e':
		//	//interrupts();
		//	E.count=0;
		//	fan.setSpeed(FanSpeed+50);
		//	break;
		//case 'r':
		//	fan.stop();
		//	break;
		//case 't':
		//	e2.magnetize();
		//	e1.demagnetize();
		//	break;
		//case '2':
		//	stp.rotateBy(-240);
		//	break;
		//case 'u':
		//	fan.setSpeed(FanSpeed);
		//	break;
		//case 'i':
		//	fan.stop();
		//	break;
		//case 'o':
		//	e3.magnetize();
		//	e2.demagnetize();
		//	break;
		//case '3':
		//	stp.rotateBy(120);
		//	break;
		//case '[':
		//	fan.setSpeed(FanSpeed);
		//	break;
		//case ']':
		//	fan.stop();
		//	break;
		//case 'a':
		//	e1.magnetize();
		//	e3.demagnetize();
		//	break;  
		//case '4': 
		//	stp.initialize();      
		case '+':
			FanSpeed += 10;
			fan.setSpeed(FanSpeed);
			break;
		case '-':
			FanSpeed -= 10;
			fan.setSpeed(FanSpeed);
			break;
		//case 'm':
		//	e1.magnetize();
		//	break;
		//case 'n':
		//	e2.magnetize();
		//	break;
		//case 'z':
		//	e3.magnetize();
		//	break;
		//case 'M':
		//	e1.demagnetize();
		//	break;
		//case 'N':
		//	e2.demagnetize();
		//	break;
		//case 'Z':
		//	e3.demagnetize();
		//	break;
		//case 'f'://while(!pole_1)
		//	{
		//		stp.initialize();
		//		e1.magnetize();
		//	}

		//	break;
		case 'I' :
		case 'i':
			pole1Init();
			break;
		case 'S':
		case 's':
			pole1Start();
			break;
		}
	}
}


