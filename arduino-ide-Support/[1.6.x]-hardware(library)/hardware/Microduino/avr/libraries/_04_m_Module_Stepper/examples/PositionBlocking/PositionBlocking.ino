// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* 单个步进电机位置控制
*/

#include <Microduino_Stepper.h>

//设置步进电机，伺服模式
StepServo stepper(PIN_DIRA, PIN_STEPA);

void setup()
{  
    stepper.begin();
    stepper.setMaxSpeed(200.0);		//设置电机最大速度
    stepper.setAcceleration(100.0);	//设置电机的加速度
}

void loop()
{    
    stepper.runToNewPosition(0);	//旋转至0步位置
    stepper.runToNewPosition(500);	//旋转至500步位置
    stepper.runToNewPosition(100);	//旋转至100步位置
    stepper.runToNewPosition(120);	//旋转至120步位置
}