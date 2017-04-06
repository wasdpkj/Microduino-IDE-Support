//MPU6050的I2C地址默认为0x68,可通过改跳线变为0x69

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;                                       //将MPU6050类命名为accelgyro

int16_t ax, ay, az;                                      //三轴加速度
int16_t gx, gy, gz;                                      //三轴角速度

void setup() {
    Wire.begin();                                        //开启 I2C 总线(主设备)
    Serial.begin(115200);                               //串口初始化，波特率为115200

    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();                              //MPU6050初始化

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
                                                        //accelgyro.testConnection(); 测试MPU6050是否连接，MPU6050连接成功返回1，失败返回0
}

void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //读取MPU6050的三轴加速度与角速度
                                                        //也可以用两个函数分别读取加速度与加速度，如下   
                                                        //accelgyro.getAcceleration(&ax, &ay, &az);
                                                        //accelgyro.getRotation(&gx, &gy, &gz);
                                                        
    //串口打印获取到的数据
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");              //x轴重力加速度
    Serial.print(ay); Serial.print("\t");              //y轴重力加速度
    Serial.print(az); Serial.print("\t");              //z轴重力加速度
    Serial.print(gx); Serial.print("\t");              //x轴角速度
    Serial.print(gy); Serial.print("\t");              //y轴角速度
    Serial.println(gz);                                 //z轴角速度
    
    delay(100);                                          //延时100ms
}

