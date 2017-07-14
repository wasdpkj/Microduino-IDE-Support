/*

  取模软件下载地址：https://wiki.microduino.cn/index.php/File:PCtolCD.zip
  取模方法：逆向（低位在前）、逐行、阴码

*/

#include <Microduino_Matrix.h>

uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {  //1x1
  {64}      //点阵IIC地址
};

Matrix display = Matrix(Addr, TYPE_COLOR); //TYPE_COLOR or TYPE_S2

static const uint8_t logoA[] PROGMEM = {
  0x00, 0x00, 0x14, 0x2A, 0x2A, 0x2A, 0x00, 0x00
};
static const uint8_t loveA[] PROGMEM = {
  0x00, 0x36, 0x49, 0x41, 0x22, 0x14, 0x08, 0x00
};
static const uint8_t loveB[] PROGMEM = {
  0x00, 0x36, 0x7F, 0x7F, 0x3E, 0x1C, 0x08, 0x00
};
static const uint8_t small[] PROGMEM = {
  0x00, 0x22, 0x55, 0x00, 0x00, 0x22, 0x1C, 0x00
};

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
  
  display.clearDisplay();
  display.setColor(0, 255, 0);
  display.drawBMP(0, 0, 8, 8, logoA);  //x,y,w,h,data
  delay(2000);
  
  display.clearDisplay();
  display.setColor(255, 0, 0);
  display.drawBMP(0, 0, 8, 8, loveA);  //x,y,w,h,data
  delay(2000);
  
  display.clearDisplay();
  display.drawBMP(0, 0, 8, 8, loveB);  //x,y,w,h,data
  delay(2000);
  
  display.clearDisplay();
  display.setColor(255, 0, 255);
  display.drawBMP(0, 0, 8, 8, small);  //x,y,w,h,data
  delay(2000);
}
