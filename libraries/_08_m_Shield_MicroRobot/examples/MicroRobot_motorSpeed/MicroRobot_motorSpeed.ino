/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @cuiwenjing@microduino.cc

   本示例给出了MicroRobot在闭环模式下驱动电机与监测实际速度的方法

   ** 带有测速信号的电机，即闭环电机，才能使用闭环模式
   ** 闭环电机也可使用开环模式控制，其实际速度和位置均可同样获取

   ** 在闭环模式下，固件里给出了一套默认的PID参数
      但因为电机型号的多样化，PID参数也需要进行相应地调节以达到更好的效果
      如需调节速度闭环PID参数，可调用以下函数进行更改
      setS_PID_P(uint8_t index, float p);
      setS_PID_I(uint8_t index, float i);
      setS_PID_D(uint8_t index, float d);

 ****************************************/


#include <Microduino_MicroRobot.h>

MicroRobot microRobot;

int16_t getSP[4], getPos[4];

void setup()
{
  //******串口初始化
  Serial.begin(115200);

  //******microRobot初始化
  microRobot.begin();

  //******电机初始化
  for (int a = 1; a <= 4; a++) {
    microRobot.motorInit(a);
  }

  //******设置电机模式、减速比、速度分辨率和初始速度
  for (int a = 1; a <= 4; a++) {
    microRobot.setMode(a, MODE_SPEED);
    microRobot.setRatio(a, 60);
    microRobot.setResolution(a, 12);
    microRobot.setSpeed(a, 0);
  }

}

void loop()
{
  //设置电机速度
  for (int a = 1; a <= 4; a++) {
    microRobot.setSpeed(a, 150);
  }

  //获取电机实际速度(r/s)*100（通过速度原始值与减速比、速度分辨率计算得出）
  Serial.print("  Speed:");
  for (int a = 1; a <= 4; a++) {
    getSP[a] = microRobot.getSpeed(a);
    Serial.print(getSP[a]);
    Serial.print(" ");
  }

  //获取电机相对速度(监测到的速度原始值)
  Serial.print("SpeedRaw:");
  for (int a = 1; a <= 4; a++) {
    getSP[a] = microRobot.getSpeedRaw(a);
    Serial.print(getSP[a]);
    Serial.print(" ");
  }

  Serial.println(" ");
  delay(20);
}

