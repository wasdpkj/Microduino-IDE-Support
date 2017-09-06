/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了点亮彩灯后，关闭所有彩灯的方法：
  使用clear()函数
  需注意的是使用此函数后，需要加show()语句
 ****************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7         //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN); //定义彩灯数量和彩灯引脚号

void setup() {
  strip.begin();           //彩灯初始化
  strip.setBrightness(80); //设置彩灯亮度
  strip.show();            //将彩灯点亮成设置的颜色
}

void loop() {
  strip.setPixelColor(0, COLOR_RED);    //设置第0号灯为红色
  strip.setPixelColor(1, COLOR_ORANGE); //设置第1号灯为橙色
  strip.setPixelColor(2, COLOR_YELLOW); //设置第2号灯为黄色
  strip.setPixelColor(3, COLOR_GREEN);  //设置第3号灯为绿色
  strip.setPixelColor(4, COLOR_CYAN);   //设置第4号灯为青色
  strip.setPixelColor(5, COLOR_BLUE);   //设置第5号灯为蓝色
  strip.setPixelColor(6, COLOR_PURPLE); //设置第6号灯为紫色
  strip.show();      //将彩灯点亮成设置的颜色
  delay(2000);       //延时2s

  strip.clear();     //清除所有灯的颜色
  strip.show();      //将彩灯点亮成设置的颜色
  delay(2000);       //延时2s
}
