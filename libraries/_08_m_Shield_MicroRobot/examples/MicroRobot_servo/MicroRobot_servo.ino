/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @cuiwenjing@microduino.cc

   本示例给出了MicroRobot控制5路舵机的方法
   舵机序号0~4
   本示例给出的是直接控制舵机角度的函数（角度设置范围：0~180）
   如需通过脉冲宽度精细调节舵机角度，可调用以下函数：
   servoWriteRaw(uint8_t index, uint16_t value);//(脉冲宽度可设范围：544~2450)

 ****************************************/

#include <Microduino_MicroRobot.h>

MicroRobot microRobot;

int pos = 0;

void setup()
{
  //******串口初始化
  Serial.begin(115200);

  //******microRobot初始化,包含舵机与PMU的初始化
  microRobot.begin();

  //******舵机接口写低电平
  for (int i = 0; i <= 4; i++) {
    microRobot.digitalWrite(i, LOW);
  }

}

void loop()
{
  //******舵机循环正反转
  for (pos = 0; pos < 180; pos += 1)
  {
    for (int i = 0; i <= 4; i++) {
      microRobot.servoWrite(i, pos);
    }
    delay(15);
  }
  for (pos = 180; pos >= 1; pos -= 1)
  {
    for (int i = 0; i <= 4; i++) {
      microRobot.servoWrite(i, pos);
    }
    delay(15);
  }

}

