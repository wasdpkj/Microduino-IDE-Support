// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别由bool变量虚拟化按键的【长按】【短按】动作
  请将碰撞开关传感器连接到A0口进行实验
*/

#include <Microduino_Key.h>

VirtualKey keyVirtual;

void setup() {
  Serial.begin(9600);
  keyVirtual.begin();
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  bool val = !digitalRead(A0);  //val 可以是任何bool变量
  switch (keyVirtual.readEvent(val)) {
    case SHORT_PRESS:
      Serial.println("KEY (Virtual) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY (Virtual) LONG_PRESS");    //长按
      break;
  }

  delay(50);
}