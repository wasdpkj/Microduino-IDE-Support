// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

#include <Microduino_Matrix.h>

uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {  //1x1
  {64}      //点阵IIC地址
};

Matrix display = Matrix(Addr, TYPE_COLOR); //TYPE_COLOR or TYPE_S2

void setup() {

  Wire.begin();

  display.setBrightness(255);
  //display.setLedBrightness(x, y, random(0, 255));

  //设置指定灯颜色
  for (int y = 0; y < display.getHeight() * 8; y++) {
    for (int x = 0; x < display.getWidth() * 8; x++) {
      display.setLedColor(x, y, random(0, 255), random(0, 255), random(0, 255));   //x, y, r, g, b
      delay(5);
    }
  }
  delay(1000);


  //设置指定灯开
  display.clearDisplay();
  for (int y = 0; y < display.getHeight() * 8; y++) {
    for (int x = 0; x < display.getWidth() * 8; x++) {
      display.setLed(x, y, true);   //x, y, sta
      delay(20);
    }
  }
  delay(1000);
  display.clearDisplay();
}

void loop() {
  // Print
  display.setColor(0, 255, 0);
  int i = display.getStringWidth("mCookie!");
  for (int a = display.getWidth() * 8; a > -i - display.getWidth() * 8; a--) {
    display.setCursor(a, 0);   //x, y
    display.print("mCookie!");
    delay(120);
  }
  display.clearDisplay();

  display.setColor(255, 0, 255);
  unsigned long timer = millis();
  while (millis() - timer < 5000) {
    display.setCursor(1, 0);   //x, y
    display.print((millis() - timer) / 500);
  }
  display.clearDisplay();

  //Char*
  display.setColor(0, 255, 255);
  display.writeString("Welcome", 120, 0); //char*, time ,y
  display.clearDisplay();

  display.setColor(0, 0, 255);
  //String 滚动方式：for循环加delay
  String _buffer_data = "Analog:";
  _buffer_data +=  analogRead(A0);

  display.writeString(_buffer_data, 80, 1);//String, time ,y
  display.clearDisplay();
}
