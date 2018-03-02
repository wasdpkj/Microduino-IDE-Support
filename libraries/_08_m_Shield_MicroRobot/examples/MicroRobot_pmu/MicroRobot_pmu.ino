/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @cuiwenjing@microduino.cc

   本示例给出了MicroRobot的PMU部分监测方法
   包括PMU固件版本号、电池电压、是否低电量状态

 ****************************************/

#include <Microduino_MicroRobot.h>

MicroRobot microRobot;

void setup()
{
  //******串口初始化
  Serial.begin(115200);

  //******microRobot初始化,包含舵机与PMU的初始化
  microRobot.begin();

}

void loop()
{
  Serial.print("Ver:");
  Serial.print(microRobot.getVersion());
  Serial.print(" Bat:");
  Serial.print(microRobot.getVoltage());
  Serial.print(" isLow:");
  Serial.print(microRobot.isLow());
  Serial.println(" ");
  delay(20);
}

