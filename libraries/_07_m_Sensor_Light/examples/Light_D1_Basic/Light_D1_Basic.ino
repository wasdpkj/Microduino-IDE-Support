/*

    模块wiki：https://wiki.microduino.cn/index.php/Sensor-Light-D1/zh
    
    获取光照强度，串口监视器打印数据。

    电路：
    * 接到核心的IIC引脚

    2017年7月11日修改
    By YLB
*/
#include <Miroduino_Light.h>

Light_D1 lightSensor;

void setup(void)
{
  Serial.begin(9600);
  bool result = lightSensor.begin(); //初始化光照传感器，检测是否连接了

  while (!result) // 当没识别到光照传感器，重复执行
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring!");//串口打印警告
    result = lightSensor.begin();  //检测是否连接了
  }

  lightSensor.enableAutoGain(true);//设置自动增益
}

void loop(void)
{
  uint32_t tslLux = lightSensor.getLuminosity(); //获取光照强度

  if (tslLux)   //如果检测到，串口打印出来
  {
    Serial.print(tslLux);
    Serial.println(" lux");
  }
  else       //否则认为光照强度超过检测范围
    Serial.println("Sensor overload");

  delay(250);
}