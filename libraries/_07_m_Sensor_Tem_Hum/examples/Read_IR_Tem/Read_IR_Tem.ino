/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_Infrared_TEM

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc
  Emissivity Table
  //Aluminum foil           | 0.03
  //Aluminum, anodized      | 0.9[11]
  //Asphalt                 | 0.88
  //Brick                   | 0.90
  //Concrete, rough         | 0.91
  //Copper, polished        | 0.04
  //Copper, oxidized        | 0.87
  //Glass, smooth (uncoated)| 0.95
  //Ice                     | 0.97
  //Limestone               | 0.92
  //Marble (polished)       | 0.89 to 0.92
  //Paint (including white) | 0.9
  //Paper, roofing or white | 0.88 to 0.86
  //Plaster, rough          | 0.89
  //Silver, polished        | 0.02
  //Silver, oxidized        | 0.04
  //Snow                    | 0.8 to 0.9
  //Water, pure             | 0.96
  //中文发射率表格：https://wenku.baidu.com/view/58333c2258fb770bf78a5581.html
  //日期：2018.2
*********************************************************/
#include <Microduino_IR_TEM.h>

IR_TEM termo;

void setup()
{
  Serial.begin(9600); //串口初始化
  Serial.println("Microduino_Sensor IR_TEM!");

  while (!termo.begin())
  {
    Serial.println("Something Wrong while Initialing");   //紫外线传感器初始化
  }
  if (termo.readID())
  {
    Serial.print("ID : 0x");
    Serial.println(String(termo.getIDH(), HEX) + String(termo.getIDL(), HEX));
  }
  //TEMP_K,TEMP_C,TEMP_F
  termo.setUnit(TEMP_C);
  //from 0.1~1.0
  termo.setEmissivity(0.7);
  Serial.print("The Emissivity is : ");
  Serial.print(termo.readEmissivity());
  Serial.println();
}
//leave the sensor about 2~3cm away
void loop()
{
  if (termo.read())
  {
    Serial.print("Object: " + String(termo.object(), 2));
    Serial.println(" C");
    Serial.print("Ambient: " + String(termo.ambient(), 2));
    Serial.println(" C");
  }
  delay(500);
}