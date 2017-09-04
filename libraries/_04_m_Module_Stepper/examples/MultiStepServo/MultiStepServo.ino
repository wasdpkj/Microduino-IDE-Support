// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* 多个步进电机位置控制， 旋转到指定位置
*/


#include <Microduino_Stepper.h>

//设置两个步进电机，伺服模式
StepServo stepperA(PIN_DIRA, PIN_STEPA);
StepServo stepperB(PIN_DIRB, PIN_STEPB);

void setup()
{
  stepperA.begin();
  stepperA.setMaxSpeed(10000.0);		//设置电机最大速度
  stepperA.setAcceleration(100.0);		//设置电机的加速度
  stepperA.moveTo(3200);				//旋转至3200步的位置			

  stepperB.begin();
  stepperB.setMaxSpeed(10000.0);		//设置电机最大速度
  stepperB.setAcceleration(100.0);		//设置电机最大加速度
  stepperB.moveTo(3200);				//旋转至3200步的位置
}

void loop()
{
  //如果旋转到指定位置
  if (stepperA.distanceToGo() == 0){
	//移动到反向的位置
    //如果当前位置是400步, 则旋转到 -400位置
    stepperA.moveTo(-stepperA.getCurrentPosition());
  }
 //这个方法需要一直调用，已确保电机平滑控制
  stepperA.run();

  if (stepperB.distanceToGo() == 0){
    stepperB.moveTo(-stepperB.getCurrentPosition());
  }
  stepperB.run();
}