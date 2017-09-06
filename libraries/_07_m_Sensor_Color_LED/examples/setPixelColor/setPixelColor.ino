/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了点亮彩灯的几种方法
 ****************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7        //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN); //将ColorLED类命名为strip，并定义彩灯数量和彩灯引脚号

void setup() {
  strip.begin();                 //彩灯初始化
  strip.setBrightness(60);       //设置彩灯亮度
  strip.show();
}

void loop() {
  //将所有灯点亮为红色
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); //（第i号灯,由strip.Color(R,G,B)产生的32位的RGB值 )
    strip.show();
  }
  delay(1000);

  //将所有灯点亮为绿色
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0, 255, 0);     //（第i号灯,R,G,B )
    strip.show();
  }
  delay(1000);

  //将所有灯点亮为蓝色
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, 0X0000FF);      //（第i号灯,32位的RGB值 )
    strip.show();
  }
  delay(1000);

  //将所有灯点亮为紫色
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, COLOR_PURPLE);  //（第i号灯,COLOR_颜色英文大写 )
    strip.show();
  }
  delay(1000);
}
