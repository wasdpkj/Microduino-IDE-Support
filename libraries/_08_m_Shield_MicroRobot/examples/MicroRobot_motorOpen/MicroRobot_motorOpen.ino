/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @cuiwenjing@microduino.cc

  本示例给出了开环模式下MicroRobot驱动电机的方法
  兼容乐高电机
  一般的N20电机、TT电机、130电机等，正确接线后均可驱动
 ****************************************/

#include <Microduino_MicroRobot.h>

MicroRobot microRobot;

void setup()
{
  //******串口初始化
  Serial.begin(115200);

  //******microRobot初始化
  microRobot.begin();

  //******电机初始化
  for (int a = 1; a <= 4; a++) {
    microRobot.motorInit(a);        //速度分辨率默认为8位，即速度可设范围-255~+255
    //    microRobot.motorInit(a,BIT_8);//速度分辨率设为8位，8~14位可设
  }

  //******设置电机模式和速度
  for (int a = 1; a <= 4; a++) {
    microRobot.setMode(a, MODE_OPEN);//电机设置为开环模式
    microRobot.setSpeed(a, 0);       //初始速度设为0
  }
}

void loop()
{
  Serial.println("Forward!");
  for (int a = 1; a <= 4; a++) {
    microRobot.setSpeed(a, 100);
  }
  delay(2000);

  Serial.println("Brake!");
  for (int a = 1; a <= 4; a++) {
    microRobot.setSpeed(a, BRAKE);
  }
  delay(2000);

  Serial.println("Back!");
  for (int a = 1; a <= 4; a++) {
    microRobot.setSpeed(a, -100);
  }
  delay(2000);

  Serial.println("Free!");
  for (int a = 1; a <= 4; a++) {
    microRobot.setSpeed(a, FREE);
  }
  delay(2000);
}

