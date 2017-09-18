// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别数字传感器的【释放】【按下】动作及状态
  请将碰撞开关传感器连接到A0口进行实验
*/

#define MODE_SWITCH

#include <Microduino_Key.h>

DigitalKey keyDigital(A0);

void setup() {
  Serial.begin(9600);
  keyDigital.begin(INPUT_PULLUP);
}

void loop() {
#if defined (MODE_SWITCH)
  switch (keyDigital.readVal()) {
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

#else
  if (keyDigital.readVal(KEY_RELEASED)) {
    Serial.println("KEY (digital) RELEASED---1");   //按键释放状态
  }
  if (keyDigital.readVal(KEY_PRESSING)) {
    Serial.println("KEY (digital) PRESSING--------3");  //按键按下动作
  }
  if (keyDigital.readVal(KEY_PRESSED)) {
    Serial.println("KEY (digital) PRESSED-------2");    //按键按下状态
  }
  if (keyDigital.readVal(KEY_RELEASING)) {
    Serial.println("KEY (digital) RELEASING---------4");    //按键释放动作
  }
#endif

  delay(100);
}