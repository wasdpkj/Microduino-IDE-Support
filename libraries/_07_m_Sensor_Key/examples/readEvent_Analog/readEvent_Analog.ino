// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别模拟传感器的【长按】【短按】动作
  请将碰撞开关传感器连接到A0口进行实验
*/

#include <Microduino_Key.h>

AnalogKey keyAnalog(A0);

void setup() {
  Serial.begin(9600);
  keyAnalog.begin(INPUT);
}

void loop() {
  switch (keyAnalog.readEvent(0, 50)) {   //(模拟最小值,模拟最大值)
    case SHORT_PRESS:
      Serial.println("KEY (analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY (analog) LONG_PRESS");    //长按
      break;
  }

  delay(50);
}