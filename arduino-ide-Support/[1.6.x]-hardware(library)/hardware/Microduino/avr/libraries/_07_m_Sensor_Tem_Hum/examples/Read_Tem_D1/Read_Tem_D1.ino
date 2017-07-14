/*
    模块墙wiki：https://wiki.microduino.cn/index.php/Sensor-Temperature-D1/zh

    获取Sensor-Temperature-D1的温度，串口监视器打印数据。并且检测传感器是否在线。

    电路：
    * 接到核心的IIC引脚

    2017年7月12日修改
    By YLB
*/
#include <Microduino_Tem_Hum.h>

Tem_D1  termo;  //调用Sensor-Temperature-D1传感器

void setup()
{
  Serial.begin(9600);
  Serial.print("Tem_D1:");
  Serial.print(termo.begin());  //初始化温度传感器，并串口打印传感器是否在线
  delay(1000);
}

void loop() 
{
  Serial.print("Tem_D1 Tem:"); 
  Serial.println(termo.getTemperature());  //串口打印获取的温度
  Serial.println("------------------");
  delay(1000);
  Serial.print("onLine:");
  Serial.println(termo.begin());  //串口打印传感器是否在线
}