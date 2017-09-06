/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  ColorLED维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Color_LED/zh

  本示例给出了获取彩灯信息的一些方法：
   彩灯级联数量（numPixels()）
   彩灯引脚号（getPin()）
   彩灯亮度（getBrightness）
   彩灯色彩值（getPixelColor）
 ****************************************/

#include <Microduino_ColorLED.h> //引用彩灯库

#define PIN            D6         //彩灯引脚
#define NUMPIXELS      7         //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN); //定义彩灯数量和彩灯引脚号

void setup() {
  Serial.begin(115200);    //串口初始化

  strip.begin();           //彩灯初始化
  strip.setBrightness(80); //设置彩灯亮度
  strip.show();            //将彩灯点亮成设置的颜色

  for (int i = 0; i < NUMPIXELS; i++) {  //将所有灯设置为蓝色
    strip.setPixelColor(i, COLOR_BLUE);
  }
  strip.show();                    //将彩灯点亮成设置的颜色
}

void loop() {
  uint8_t a = strip.numPixels();  //将获取到的已设置的彩灯级联数量值赋给a
  Serial.print("numPixels: ");
  Serial.print(a);               //串口打印a的值

  int8_t b = strip.getPin();      //将获取到的彩灯引脚号的值赋给b
  Serial.print("   Pin: ");
  Serial.print(b);               //串口打印b的值

  uint8_t c = strip.getBrightness(); //将获取到的彩灯亮度值赋给c
  Serial.print("   Brightness: ");
  Serial.print(c);                  //串口打印c的值

  uint32_t d = strip.getPixelColor(0); //将获取到的第0号彩灯色彩值赋给d，设置的亮度不同，获取到的值也会不同
  Serial.print("   PixelColor(0): ");
  Serial.println(d, HEX);             //串口打印d的16进制值

  delay(100);                         //延时100ms
}
