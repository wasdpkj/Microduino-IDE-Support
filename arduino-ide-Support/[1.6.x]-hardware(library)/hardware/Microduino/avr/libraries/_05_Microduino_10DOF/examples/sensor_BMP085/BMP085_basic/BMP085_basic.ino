/*BMP085数字气压传感器,集成在motion(又名10dof)模块里，Microduino系列与mCookie系列均有，I2C address is 0x77*/

#include "BMP085.h"                     //头文件

BMP085 barometer;                       //将BMP085类命名为barometer

uint8_t bmpMode;                        //BMP085的模式
float temperature;                      //温度
float pressure;                         //气压
float altitude;                         //海拔

void setup() {
    Serial.begin(115200);              //串口初始化，波特率为115200
    
    Serial.println("Initializing I2C devices...");
    Serial.println(barometer.begin() ? "BMP085 connection successful" : "BMP085 connection failed");
                                       //barometer.begin(); BMP085初始化，此初始化函数有返回值，返回1 BMP085连接成功，返回0失败

    bmpMode = BMP085_MODE_PRESSURE_3;  //设置BMP模式为BMP085_MODE_PRESSURE_3，BMP085_MODE_TEMPERATURE（温度模式），
                                       //BMP085_MODE_PRESSURE_0、BMP085_MODE_PRESSURE_1、BMP085_MODE_PRESSURE_2、BMP085_MODE_PRESSURE_3（从_0到_3，传感器精度越来越高） 
}

void loop() {
  
    while(!barometer.available(BMP085_MODE_PRESSURE_3));  // 等待BMP085气压传感器以设定好的模式工作 
    
    temperature = barometer.readTemperature();  //读取温度值（。C）
    pressure = barometer.readPressure();        //读取气压值(Pa)
    altitude = barometer.getAltitude(pressure); //通过读取的气压值对比海平面气压值（101325）算得当前海拔(m)

    //将上面读取的数值串口打印出来
    Serial.print("T/P/A\t");  
    Serial.print(temperature); Serial.print("\t");
    Serial.print(pressure); Serial.print("\t");
    Serial.print(altitude);
    Serial.println("");
 
    delay(100);                                //延时100ms
}
