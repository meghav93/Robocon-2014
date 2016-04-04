/* Robocon 2014, Theme by India. A Salute to Parenthood.
   G.H. Patel College of Engineering and Technology.
   Code for Parent Robot. Theme @ https://www.youtube.com/watch?v=02QLk3Pu9UY
   Author :- Meghav Verma, 4th Year, Electronics & Communication, Team Lead. */



#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "Declarations.h"

int moveDirection = 0;
encoder E(38, 39),E2(31,30);
PIDMotor M(9, 8, &E2),M2(6,7,&E);
boolean flag=false;
int sp=0;


void setup()
{
  Serial.begin(9600);
  Serial.flush();
  Serial.print((char)35);
  M2.makeSlave(&M);
  M2.kp = 0.5;
  M2.ki = -0.01;
  M2.kd = 0.8;
  delay(5000);
  M.addToTarget(44*1440);
        M2.addToTarget(-44*1440);
}

long int lastAngle = -999,lastAngle2 = -999;

void loop()
{  
  encoder::updateAll();
  M.update();
  M2.update();
  /*if (lastAngle != E.lastVal) {
   		lastAngle = E.lastVal;
   		Serial.print(lastAngle);
   Serial.print(",");
   Serial.print(E2.lastVal);
   Serial.print(",");
   lastAngle = E.lastVal;
   	}*/if(flag){
          Serial.print(M2.error); Serial.print(",");}
  if (Serial.available())
    switch(Serial.read()) 
    {       
    case '1': 
      {
        M.addToTarget(4.35*1440);
        M2.addToTarget(-4.35*1440);
        flag=true;
        break;
      }
    case '2':
      {
        M.addToTarget(-14.5*1440);
        M2.addToTarget(-10);
        flag = true;
        break;
      } 
      case '0':
      {
        M.stop();
        M2.stop();
        flag = false;
        break;
      }
    case 'd' :
      {
        M2.kd+=0.1;
        break;
      }
    case 'D':
      {
        M2.kd -= 0.1;
        break;
      }
    case 'i':
      {
        M2.ki += 0.01;
        break;
      }
    case 'I':
      {
        M2.ki -= 0.01;
        break;
      }
    case 'p':
      {
        M2.kp += 0.1;
        break;
      }
    case 'P':
      {
        M2.kp -= 0.1;
        break;
      }
    case 's':
      {
        sp+=1;
      }
    case 'S':
      {
        sp-=1;
      }
     default:
      Serial.print(M2.kp);
      Serial.print("  ");
      Serial.print(M2.ki);
      Serial.print("  ");
      Serial.println(M2.kd);
      break;

    }
  //Serial.println(E.read());
}



