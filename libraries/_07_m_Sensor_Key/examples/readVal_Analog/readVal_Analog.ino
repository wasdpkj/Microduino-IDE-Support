// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别模拟传感器的【释放】【按下】动作及状态
  请将碰撞开关传感器连接到A0口进行实验
*/
//#define MODE_SWITCH

#include <Microduino_Key.h>

AnalogKey keyAnalog(A0);

void setup() {
  Serial.begin(9600);
  keyAnalog.begin(INPUT);
}

void loop() {
#if defined (MODE_SWITCH)
  switch (keyAnalog.readVal(0, 50)) {  //(模拟最小值,模拟最大值)
    case KEY_RELEASED:
      Serial.println("KEY (analog)  RELEASED---1");     //按键释放状态
      break;
    case KEY_PRESSED:
      Serial.println("KEY (analog)  PRESSED-------2");    //按键按下状态
      break;
    case KEY_PRESSING:
      Serial.println("KEY (analog)  PRESSING--------3");    //按键按下动作
      break;
    case KEY_RELEASING:
      Serial.println("KEY (analog)  RELEASING---------4");    //按键释放动作
      break;
  }
#else
  if (keyAnalog.readVal(0, 50, KEY_RELEASED)) {
    Serial.println("KEY (analog) RELEASED---1");   //按键释放状态
  }
  if (keyAnalog.readVal(0, 50, KEY_PRESSING)) {
    Serial.println("KEY (analog) PRESSING--------3");  //按键按下动作
  }
  if (keyAnalog.readVal(0, 50, KEY_PRESSED)) {
    Serial.println("KEY (analog) PRESSED-------2");    //按键按下状态
  }
  if (keyAnalog.readVal(0, 50, KEY_RELEASING)) {
    Serial.println("KEY (analog) RELEASING---------4");    //按键释放动作
  }
#endif

  delay(100);
}