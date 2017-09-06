/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了实现呼吸灯效的方法
 ****************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7         //级联彩灯数量

#define val_max 255
#define val_min 0

ColorLED strip = ColorLED(NUMPIXELS, PIN); //将ColorLED类命名为strip，并定义彩灯数量和彩灯引脚号

void setup() {
  strip.begin();              //彩灯初始化
  strip.show();
}

void loop() {
  rainbowCycle( 255, 0, 0, 5);//红色呼吸
  rainbowCycle( 0, 255, 0, 5);//绿色呼吸
  rainbowCycle( 0, 0, 255, 5);//蓝色呼吸
}

void colorSet(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void rainbowCycle( int r, int g, int b, uint8_t wait) {
  for (int val = 0; val < 255; val++)
  {
    colorSet(strip.Color(map(val, val_min, val_max, 0, r), map(val, val_min, val_max, 0, g), map(val, val_min, val_max, 0, b)));
    delay(wait);
  }
  for (int val = 255; val >= 0; val--)
  {
    colorSet(strip.Color(map(val, val_min, val_max, 0, r), map(val, val_min, val_max, 0, g), map(val, val_min, val_max, 0, b)));
    delay(wait);
  }
}
