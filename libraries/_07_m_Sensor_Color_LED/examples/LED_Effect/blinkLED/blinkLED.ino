/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了实现闪烁灯效的方法
 ****************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7         //级联彩灯数量

#define val_max 255
#define val_min 0

ColorLED strip = ColorLED(NUMPIXELS, PIN); //将ColorLED类命名为strip，并定义彩灯数量和彩灯引脚号

void setup() {
  strip.begin();   //彩灯初始化
  strip.show();
}

void loop() {
  ledBlinkALL(COLOR_BLUE, 1000); //所有灯蓝色闪烁
}

//第n号灯，颜色c,闪烁时间wait
void ledBlink(uint32_t c, uint8_t n, uint8_t wait) {
  strip.setPixelColor(n, c);  
  strip.show();
  delay(wait);
  strip.setPixelColor(n, 0); 
  strip.show();
  delay(wait);
}

//全部灯，颜色c,闪烁时间wait
void ledBlinkALL(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
  delay(wait);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
  }
  delay(wait);
}
