// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

#include <Microduino_Matrix.h>

uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {  //1x1
  {64}      //点阵IIC地址
};

Matrix display = Matrix(Addr, TYPE_S2); //TYPE_COLOR or TYPE_S2

void setup() {

  Wire.begin();

  display.setBrightness(255);
  //display.setLedBrightness(x, y, random(0, 255));

  //setLed
  display.clearDisplay();
  for (int y = 0; y < display.getHeight() * 8; y++) {
    for (int x = 0; x < display.getWidth() * 8; x++) {
      display.setLed(x, y, true);   //x, y, sta
      delay(20);
    }
  }
  delay(100);
  display.clearDisplay();
}

void loop() {
  // Print
  int i = display.getStringWidth("mCookie!");
  for (int a = display.getWidth() * 8; a > -i - display.getWidth() * 8; a--) {
    display.setCursor(a, 0);   //x, y
    display.print("mCookie!");
    delay(120);
  }
  display.clearDisplay();

  unsigned long timer = millis();
  while (millis() - timer < 5000) {
    display.setCursor(1, 0);   //x, y
    display.print((millis() - timer) / 500);
  }
  display.clearDisplay();

  //Char*
  display.writeString("Welcome", 120, 0); //char*, time ,y
  display.clearDisplay();

  //String 滚动方式：for循环加delay
  String _buffer_data = "Analog:";
  _buffer_data +=  analogRead(A0);

  display.writeString(_buffer_data, 80, 1);//String, time ,y
  display.clearDisplay();
}
