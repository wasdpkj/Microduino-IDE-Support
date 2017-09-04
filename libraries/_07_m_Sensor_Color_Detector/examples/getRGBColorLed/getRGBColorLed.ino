#include <Microduino_ColorLED.h> //引用彩灯库
#include <Microduino_ColorDetector.h>

#define PIN            6         //彩灯引脚
#define NUMPIXELS      3        //级联彩灯数量

ColorLED strip = ColorLED(NUMPIXELS, PIN); //将ColorLED类命名为strip，并定义彩灯数量和彩灯引脚号

uint16_t red, green, blue, clr,led;

ColorDetector colorSensor;

void setup() {
  Serial.begin(115200);
  Serial.print("ColorDetector start!\n");

  strip.begin();                 //彩灯初始化
  //strip.setBrightness(60);
  strip.show();

  colorSensor.begin();
  //  tcs.setIntegrationTime(INTEG_PARAM_INTTIME_12MS);
  //  tcs.setGain(GAIN_1, PRESCALER_1);
  //  tcs.start();
}

void loop() {
  delay(100);  // normally you should wait at least the equivalent of integration time (set at 12MS above)
  colorSensor.getRGB(&red, &green, &blue, &clr);
  red = map(red, 0, 32767, 0, 255);
  green = map(green, 0, 32767, 0, 255);
  blue = map(blue, 0, 32767, 0, 255);

  led=map(analogRead(A0),0,1023,0,255);

  delay(300);

  Serial.print("Red:");
  Serial.print(red, DEC);
  Serial.print("\tGreen:");
  Serial.print(green, DEC);
  Serial.print("\tBlue:");
  Serial.print(blue, DEC);
  Serial.print("\tClear:");
  Serial.print(clr, DEC);
  Serial.write('\n');
  strip.setPixelColor(2, strip.Color(red, green, blue)); //（第i号灯,由strip.Color(R,G,B)产生的32位的RGB值 )
  strip.setPixelColor(1, strip.Color(led, led, led));
  strip.setPixelColor(0, strip.Color(led, led, led));
  strip.show();
}
