/*
    模块墙wiki：https://wiki.microduino.cn/index.php/Sensor-Tem%26Hum-S2/zh

    获取Sensor-Tem&Hum-S2的温湿度，串口监视器打印数据。并且检测传感器是否在线。

    电路：
      接到核心的IIC引脚

    2017年7月12日修改
    By YLB
*/

#include <Microduino_Tem_Hum.h>

Tem_Hum_S2  termo; //调用Sensor-Tem&Hum-S2传感器

void setup()
{
  Serial.begin(9600);
  Serial.print("Tem_Hum_S2:");
  Serial.print(termo.begin());  //初始化温湿度传感器，并串口打印传感器是否在线
  delay(1000);
}

void loop()
{
  Serial.print("Tem_Hum_S2 Tem:");
  Serial.print(termo.getTemperature()); //串口打印获取的温度
  Serial.print("\t");
  Serial.print("Tem_Hum_S2 Hum:");
  Serial.println(termo.getHumidity()); //串口打印获取的湿度
  Serial.println("------------------");
  delay(1000);
  Serial.print("onLine:");
  Serial.println(termo.begin());  //串口打印传感器是否在线
}
