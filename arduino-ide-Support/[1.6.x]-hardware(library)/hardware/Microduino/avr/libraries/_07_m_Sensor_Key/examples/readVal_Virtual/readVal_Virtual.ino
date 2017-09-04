// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别由bool变量虚拟化按键的【释放】【按下】动作及状态
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
  switch (keyVirtual.readVal(val)) {
    case KEY_RELEASED:
      Serial.println("KEY (digital) RELEASED---1");   //按键释放状态
      break;
    case KEY_PRESSED:
      Serial.println("KEY (digital) PRESSED-------2");    //按键按下状态
      break;
    case KEY_PRESSING:
      Serial.println("KEY (digital) PRESSING--------3");  //按键按下动作
      break;
    case KEY_RELEASING:
      Serial.println("KEY (digital) RELEASING---------4");    //按键释放动作
      break;
  }

  delay(100);
}