/*
  模块墙wiki：https://wiki.microduino.cn/index.php/Sensor-Tem%26Hum/zh

  获取Sensor-Tem&Hum的温湿度，串口监视器打印数据。并且检测传感器是否在线。

  电路：
  接到核心的IIC引脚

  2017年7月12日修改
  By YLB
*/

#include <Microduino_Tem_Hum.h>

Tem_Hum  termo; //调用Sensor-Tem&Hum传感器

void setup()
{
  Serial.begin(9600);
  Serial.print("Tem_Hum:");
  Serial.print(termo.begin());
  delay(1000);
}

void loop()
{
  Serial.print("Tem_Hum Tem:");
  Serial.print(termo.getTemperature()); //串口打印获取的温度
  Serial.print("\t");
  delay(500);
  Serial.print("Tem_Hum Hum:");
  Serial.println(termo.getHumidity()); //串口打印获取的湿度
  Serial.println("------------------");
  delay(500);
  Serial.print("onLine:");
  Serial.println(termo.begin());  //串口打印传感器是否在线
}
