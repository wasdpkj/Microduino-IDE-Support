/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @cuiwenjing@microduino.cc

   本示例给出了MicroRobot位置闭环模式下驱动电机的方法

   ** 带有测速信号的电机，即闭环电机，才能使用闭环模式
   ** 闭环电机也可使用开环模式控制，速度和位置均可获取

   ** 在闭环模式下，固件里给出了一套默认的PID参数
      但因为电机型号的多样化，PID参数也需要进行相应地调节
      如需调节位置闭环PID参数，可调用以下函数进行更改
      setP_PID_P(uint8_t index, float p);
      setP_PID_I(uint8_t index, float i);
      setP_PID_D(uint8_t index, float d);

 ****************************************/


#include <Microduino_MicroRobot.h>

MicroRobot microRobot;

int16_t getPos[4], setPos;

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

  //******设置电机模式、减速比、速度分辨率和初始位置
  for (int a = 1; a <= 4; a++) {
    microRobot.setMode(a, MODE_POSITION);
    microRobot.setRatio(a, 60);
    microRobot.setResolution(a, 12);
    microRobot.setPosition(a, 0);
  }

}

void loop()
{
  setPos = 100;
  //  setPos = map(analogRead(A0), 0, 1023, 50, 550);//如果A0接旋转电位计可用此句替代
  
  //打印设置的电机位置
  Serial.print("setPos: ");
  Serial.print(setPos);
  Serial.print("  ");

  //设置电机位置
  for (int a = 1; a <= 4; a++) {
    microRobot.setPosition(a, setPos);
  }

  //获取实际电机位置
  Serial.print("getPos: ");
  for (int a = 1; a <= 4; a++) {
    getPos[a] = microRobot.getPosition(a);
    Serial.print(getPos[a]);
    Serial.print(" ");
  }

  Serial.println(" ");
  delay(20);
}

