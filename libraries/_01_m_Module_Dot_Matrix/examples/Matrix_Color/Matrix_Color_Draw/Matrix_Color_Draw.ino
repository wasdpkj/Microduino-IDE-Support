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
  //画线，起点坐标x,y  终点坐标x1,y1
  display.setColor(255, 0, 0);
  display.drawLine(0, 0, 7, 7); //x,y,x1,y1
  delay(2000);
  display.clearDisplay();

  display.drawLine(7, 0, 0, 7); //x,y,x1,y1
  delay(2000);
  display.clearDisplay();

  //画实心距形，起点坐标x,y 宽高w,h
  display.setColor(0, 255, 0);
  display.drawBox(0, 0, 8, 8);  //x,y,w,h
  delay(2000);
  display.clearDisplay();
  //画实心距形加倒角，起点坐标x,y 宽高w,h 倒角r
  display.drawRBox(0, 0, 8, 8, 2);  //x,y,w,h,r
  delay(2000);
  display.clearDisplay();

  //画空心距形，起点坐标x,y 宽高w,h
  display.setColor(0, 0, 255);
  display.drawFrame(0, 0, 8, 8);  //x,y,w,h
  delay(2000);
  display.clearDisplay();
  //画空心距形加倒角，起点坐标x,y 宽高w,h 倒角r
  display.drawRFrame(0, 0, 8, 8, 2);  //x,y,w,h,r
  delay(2000);
  display.clearDisplay();

  //画空心圆，圆心x,y 半径r
  display.setColor(255, 0, 255);
  display.drawCircle(3, 3, 3);  //x,y,r
  delay(2000);
  display.clearDisplay();
  //画空心圆，圆心x,y 半径r
  display.drawDisc(3, 3, 3);  //x,y,r
  delay(2000);
  display.clearDisplay();
}
