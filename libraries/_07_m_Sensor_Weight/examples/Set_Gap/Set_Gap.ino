/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_Weight
  //该示例用于校准传感器称重系数
  //首先要持有一个已知重量的物体，示例中为标准1KG砝码。
  //打开串口
  //根据提示首先清空传感器保证上面没放东西，按下按键（短按，切不可长按）
  //之后将标准重量的物体放置在传感器上，按下按键（长按短按均可）
  //校准成功，之后串口开始返回数值
  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc
  //日期：2018.8
*********************************************************/
#include "Sensor_Weight.h"

SensorWeight WT(WEIGHT_ADDRESS);

#define ITEM_WEIGHT 1000	//change this variable to your own item weight
#define BUTTON	D8			//this is a crash sensor or a normal button

uint16_t gap = 0;
uint16_t error_t;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!WT.begin())
  {
    Serial.println("Error while initialling...");
  }
  pinMode(BUTTON, INPUT_PULLUP);  //connect a button to D8
  Serial.println("Make sure nothing is on the Weitght Sensor.");
  Serial.println("Then press the button connected to D8");
  while (digitalRead(BUTTON));
  delay(1000);
  WT.getMaopi();
  Serial.println("Put the item on the Weight Sensor.");
  Serial.println("Then press the button connected to D8");
  while (digitalRead(BUTTON));
  error_t = WT.calibration(ITEM_WEIGHT);//not heavy than 2KG
  if(!error_t)
  {
	Serial.println("item too heavy");
  }
  gap = WT.getGap();
  Serial.print("Gap set : ");
  Serial.println(gap);
}

void loop() {
  Serial.print("OUTPUT : ");
  Serial.println(WT.getWeight());
  //A more-than-100MS delay is highly recommended since the Firmware based on HX711 got a sequence of 10Hz.
  delay(400);
}