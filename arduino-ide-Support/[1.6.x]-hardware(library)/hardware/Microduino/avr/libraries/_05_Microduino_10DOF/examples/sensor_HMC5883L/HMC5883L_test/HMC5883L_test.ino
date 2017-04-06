/*
Microduino系列的motion(又名10dof)模块里集成了HMC5883L磁场传感器,可获取磁场数据
mCookie系列的motion模块由于装配含有磁铁，会影响磁场，所以取掉了HMC5883L磁场传感器
*/

#include "HMC5883L.h"                        //头文件
HMC5883L mag;                                //将HMC5883L类命名为mag

int16_t x, y, z;                             //定义变量，磁场强度原始值
float mx, my, mz;                            //定义变量，增益后的磁场强度
float dx, dy, dz;                            //定义变量，由磁场算出的角度

void setup() {
  Serial.begin(115200);                     //串口初始化，波特率为115200

  Serial.println("Initializing I2C devices...");
  Serial.println(mag.begin() ? "HMC5883L connection successful" : "HMC5883L connection failed"); 
                                             //mag.begin(); HMC5883L磁场传感器初始化，此初始化函数有返回值，返回1 HMC5883L连接成功，返回0失败
  
  Serial.println("Calibrate start, please roate the sensor in 20s ...");
  mag.calibrateMag(0);                       //磁场校准，需要用户在三个空间方向翻转模块以进行校准（重要）
  Serial.println("Calibrate done.");
  Serial.print("offser:\t");
  Serial.print(mag.xOffset); Serial.print("\t");  //x轴磁场偏移值,由校准时得到
  Serial.print(mag.yOffset); Serial.print("\t");  //y轴磁场偏移值
  Serial.println(mag.zOffset);                     //z轴磁场偏移值
}

void loop() {
  mag.getHeading(&x, &y, &z);                //获取磁场强度原始值（去除偏移值后的）
  Serial.print("heading:\t");
  Serial.print(x); Serial.print("\t");     //串口打印x轴原始磁场强度
  Serial.print(y); Serial.print("\t");     //串口打印y轴原始磁场强度
  Serial.print(z); Serial.print("\t");     //串口打印z轴原始磁场强度

  mag.getMagneto(&mx, &my, &mz);            //获取增益后的磁场强度（增益值可通过setGain(uint8_t gain)来设）
  Serial.print("mag:\t");
  Serial.print(mx); Serial.print("\t");   //串口打印x轴磁场强度
  Serial.print(my); Serial.print("\t");   //串口打印y轴磁场强度
  Serial.print(mz); Serial.print("\t");   //串口打印z轴磁场强度

  mag.getDegrees(&dx, &dy, &dz);            //获取由磁场算出的角度值
  Serial.print("degrees:\t");
  Serial.print(dx); Serial.print("\t");   //串口打印x轴角度（偏航），0-360度
  Serial.print(dy); Serial.print("\t");   //串口打印y轴角度(俯仰)，0-360度
  Serial.println(dz);                      //串口打印z轴角度(滚转)，0-360度

  delay(100);
}
