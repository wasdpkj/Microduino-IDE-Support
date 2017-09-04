// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.cc
// ==============

/*
* 单个步进电机位置控制，旋转到指定位置
*/


#include <Microduino_Stepper.h>

//设置步进电机，伺服模式
StepServo stepper(PIN_DIRA, PIN_STEPA);

void setup() {
  stepper.begin();
  stepper.setMaxSpeed(1960);		//设置电机最大速度
  stepper.setSpeed(1960);			//设置电机的速度
  stepper.setAcceleration(8000);  	//设置电机的加速度
  stepper.moveTo(320); 				//旋转至320步的位置  
}

void loop() {
  //如果旋转到指定位置
  if (stepper.distanceToGo() == 0) {
    //移动到反向的位置
    //如果当前位置是400步, 则旋转到 -400位置
    stepper.moveTo(-stepper.getCurrentPosition());
  }

  //这个方法需要一直调用，已确保电机平滑控制
  stepper.run();
}