/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了更改设置的彩灯级联数的方法
  updateLength(NUMPIXELS)
 ****************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7        //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN); //定义彩灯数量和彩灯引脚号

void setup() {
  strip.begin();      //彩灯初始化
  strip.show();      //将彩灯点亮成设置的颜色
}

void loop() {
  /*将彩灯级联数更改为3，设置所有彩灯的颜色并点亮，此时只有3个灯被点亮*/
  strip.updateLength(3);   //更改级联彩灯数量为3
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, COLOR_BLUE);
  }
  strip.show();      //将彩灯点亮成设置的颜色
  delay(2000);       //延时2s

  /*关闭所有彩灯，延时1S*/
  strip.clear();     //清除所有灯的颜色
  strip.show();      //将彩灯点亮成设置的颜色
  delay(1000);       //延时1s

  /*将彩灯级联数更改为7，设置所有彩灯的颜色并点亮，此时有7个灯被点亮*/
  strip.updateLength(7); //更改级联彩灯数量为7
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, COLOR_BLUE);
  }
  strip.show();      //将彩灯点亮成设置的颜色
  delay(2000);       //延时2s

  /*关闭所有彩灯，延时1S*/
  strip.clear();     //清除所有灯的颜色
  strip.show();      //将彩灯点亮成设置的颜色
  delay(1000);       //延时1s
}
