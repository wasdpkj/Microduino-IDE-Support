#include <Microduino_Stepper.h>


StepMotor stepperL(PIN_DIRA, PIN_STEPA);
StepMotor stepperR(PIN_DIRD, PIN_STEPD);


void setup()
{
  Serial.begin(115200);
  Serial.println("Microduino!");

  stepperL.begin();       //左电机初始化
//stepperL.begin(1024);	  //设置电机最高速度
  stepperR.begin();       //右电机初始化
//stepperR.begin(1024);	  //设置电机最高速度

  delay(2000);
}

void loop()
{
  stepperR.setSpeed(60);
  stepperL.setSpeed(-60);
  delay(5000);
  stepperR.setSpeed(-60);
  stepperL.setSpeed(60);
  delay(5000);
}