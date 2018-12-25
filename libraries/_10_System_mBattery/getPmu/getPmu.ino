/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @cuiwenjing@microduino.cc

   本示例给出了mBattery的PMU部分监测方法
   包括PMU固件版本号、电池电压、是否低电量状态

 ****************************************/

#include <mBattery_Pmu.h>

MbaPmu pmu;

void setup()
{
  //******串口初始化
  Serial.begin(115200);

  //******PMU的初始化
  pmu.begin();

}

void loop()
{
  Serial.print("Ver:");
  Serial.print(pmu.getVersion());
  Serial.print(" Bat:");
  Serial.print(pmu.getVoltage(3.3));  //3.3为PMU芯片电压
  Serial.print(" isLow:");
  Serial.print(pmu.isLow());
  Serial.println(" ");
  delay(20);
}
