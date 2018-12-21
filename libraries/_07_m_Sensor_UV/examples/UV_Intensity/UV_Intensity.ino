/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_UV

  // Microduino wiki:
  // http://wiki.microduino.cn
	//根据《世界健康组织》、《世界气象学组织》、《联合国环境组织》等2002年新制定的紫外线指数标准，紫外线强度划分为弱、中等、强、很强和极强5个等级。

	//1、0-2    1级   最弱    安全 可以不采取措施。

	//2、3-5    2级     弱      正常 外出戴防护帽或太阳镜。　

	//3、6-7    3级   中等    注意 除戴防护帽和太阳镜外涂擦防晒霜(防晒霜SPF指数应不低于15)。　

	//4、0-10    4级     强     较强 在上午十点至下午四点时段避免外出活动，外出时应尽可能在遮荫处。　　

	//5、>10    5级   很强    有害 尽量不外出，必须外出时，要采取一定的防护措施。
  // E-mail:
  // jinqipeng@mircoduino.cc
  UV Intensity level:
  0.0~2.9   LOW
  3.0~5.9   Moderate 
  6.0~7.9   High
  8.0~10.9  Very High
  11.0+     Extreme
  //日期：2018.2
*********************************************************/
#include <Microduino_Sensor-UV.h>
SensorUV UV(UV_ADDRESS);
//CHECK THE +3.3V VOLTAGE ON YOUR BOARD AND FIX VREF TO GET A CORRECT FEED IF NEEDED.
#define VREF 3.34
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //串口初始化
  Serial.println("Microduino_Sensor UV!");
  while (!UV.begin())
  {
    Serial.println("Something Wrong while Initialing");   //紫外线传感器初始化
  }
  Serial.print("Version : ");
  Serial.println(UV.getVersion());
}

void loop() {
  Serial.print("Intensity : ");
  Serial.println(UV.getIntensity(),1);
  switch(UV.getUVIndex()){
  case 1:
	Serial.println("LOW");
	break;
  case 2:
  	Serial.println("MODERATE");
	break;
  case 3:
  	Serial.println("NORMAL");
	break;
  case 4:
  	Serial.println("STRONG");
	break;
  case 5:
  	Serial.println("EXTREME");
	break;
	default:
	break;
  }
  delay(400);
}