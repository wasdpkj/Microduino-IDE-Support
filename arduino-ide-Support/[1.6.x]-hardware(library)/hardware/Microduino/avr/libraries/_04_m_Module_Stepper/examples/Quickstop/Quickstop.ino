// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* 单个步进电机位置控制， 急停功能
*/


#include <Microduino_Stepper.h>

//设置步进电机，伺服模式
StepServo stepper(PIN_DIRA, PIN_STEPA);

void setup()
{  
  stepper.begin();
  stepper.setMaxSpeed(150);		//设置电机最大速度
  stepper.setAcceleration(100);	//设置电机的加速度
}

void loop()
{    
  stepper.moveTo(500);			//旋转至500步的位置
  while (stepper.getCurrentPosition() != 300) //等待旋转至300步位置
    stepper.run();
  stepper.stop(); // 快速停止
  stepper.runToPosition(); 

  //移动到反向的位置
  //如果当前位置是400步, 则旋转到 -400位置
  stepper.moveTo(-500);
  while (stepper.getCurrentPosition() != 0) //等待旋转至0步位置
    stepper.run();
  stepper.stop(); // 快速停止
  stepper.runToPosition(); 
  // Now stopped after quickstop
}