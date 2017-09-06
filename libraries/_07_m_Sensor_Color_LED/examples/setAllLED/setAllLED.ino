/*******************************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了点亮单个彩灯、全部彩灯和关闭所有彩灯的方法
  备注：setOneLED()与setPixelColor()等效
 ******************************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7         //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN); //将ColorLED类命名为strip，并定义彩灯数量和彩灯引脚号

void setup() {
  strip.begin();                 //彩灯初始化
  strip.setBrightness(60);       //设置彩灯亮度
  strip.show();
}

void loop() {
  //将所有灯点亮为红色
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setOneLED(i, COLOR_RED);  //点亮单个彩灯,第i号彩灯为红色
    strip.show();
  }
  delay(1000);

  //将所有灯点亮为绿色
  strip.setAllLED(COLOR_GREEN);
  strip.show();
  delay(1000);

  //将所有灯点亮为蓝色
  strip.setAllLED(0, 0, 255);
  strip.show();
  delay(1000);

  //关闭所有彩灯
  strip.setAllLED(COLOR_NONE);
  strip.show();
  delay(1000);
}
