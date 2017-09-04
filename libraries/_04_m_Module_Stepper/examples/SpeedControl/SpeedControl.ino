// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* 单个步进电机速度控制
*/

#include <Microduino_Stepper.h>

//设置两个步进电机，速度模式
Stepper stepperL(PIN_DIRA, PIN_STEPA);
Stepper stepperR(PIN_DIRD, PIN_STEPD);


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
  stepperR.setSpeed(60);	//设置速度
  stepperL.setSpeed(-60);
  delay(5000);
  stepperR.setSpeed(-60);
  stepperL.setSpeed(60);
  delay(5000);
}