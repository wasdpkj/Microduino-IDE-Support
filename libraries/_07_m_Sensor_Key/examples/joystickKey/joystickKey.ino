// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
  本例程可以识别Sensor Joystick传感器的【长按】【短按】动作
  请将碰撞开关传感器连接到A0口进行实验
*/

#include <Microduino_Key.h>

enum KeyName {
  UP, DOWN, LEFT, RIGHT, PRESS
};

AnalogKey keyAnalog[5] {(A0), (A0), (A0), (A0), (A0)};


void setup() {
  Serial.begin(9600);
  for (uint8_t a = 0; a < 5; a++) {
    keyAnalog[a].begin(INPUT);
  }
#if defined (ESP32)
  analogReadResolution(10);  //9-12 在此ADC设置为10bit精度
#endif
}

void loop() {
  switch (keyAnalog[UP].readEvent(700 - 50, 700 + 50)) {
    case SHORT_PRESS:
      Serial.println("KEY UP(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY UP(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyAnalog[DOWN].readEvent(330 - 50, 330 + 50)) {
    case SHORT_PRESS:
      Serial.println("KEY DOWN(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY DOWN(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyAnalog[LEFT].readEvent(512 - 50, 512 + 50)) {
    case SHORT_PRESS:
      Serial.println("KEY LEFT(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY LEFT(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyAnalog[RIGHT].readEvent(860 - 50, 860 + 50)) {
    case SHORT_PRESS:
      Serial.println("KEY RIGHT(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY RIGHT(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyAnalog[PRESS].readEvent(0, 50)) {
    case SHORT_PRESS:
      Serial.println("KEY PRESS(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY PRESS(analog) LONG_PRESS");    //长按
      break;
  }
  delay(50);
}