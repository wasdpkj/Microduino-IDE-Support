// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别数字传感器的【长按】【短按】动作，并且可以配置长按的时间
  请将碰撞开关传感器连接到A0口进行实验
*/

#include <Microduino_Key.h>

DigitalKey keyDigital(A0);

void setup() {
  Serial.begin(9600);
  keyDigital.begin(INPUT_PULLUP);
}

void loop() {
  switch (keyDigital.readEvent(3000)) {   //长按时间单位ms，不填为1000
    case SHORT_PRESS:
      Serial.println("KEY (digital) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY (digital) LONG_PRESS");    //长按
      break;
  }

  delay(50);
}