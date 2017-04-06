
#include "MPU6050_6Axis_Microduino.h"
#include "HMC5883L.h"

//#define AXIS_9                       //如果用Microduino系列的motion需要定义此句（用来进行磁场校准），如果用mCookie的motion不用定义此句，隐去就行

MPU6050 mpu;                           //将MPU6050类命名为mpu

#ifdef AXIS_9                          //如果定义了AXIS_9，就启用HMC5883L磁场传感器，没定义就不启用
  HMC5883L mag;                        //这是因为Microduino系列的motion有磁场传感器，mCookie的motion没有磁场传感器
#endif

//#define OUTPUT_READABLE_QUATERNION   //如果定义这句，可查询实际的四元数数据[w, x, y, z]
#define OUTPUT_READABLE_YAWPITCHROLL   //如果定义这句，得到的是由四元数算得的偏航、俯仰、滚转的角度值

Quaternion q;                          // [w, x, y, z]         四元数
float ypr[3];                          // [yaw, pitch, roll]   yaw/pitch/roll（偏航、俯仰、滚转）的角度值
float mx, my, mz;                      //磁场强度
uint8_t mpuMode;                       //MPU6050模式
bool mpuReady;                         //mpu初始化状态值

void setup() {
    Serial.begin(115200);             //串口初始化，波特率为115200
    while (!Serial);                  //打开串口监视器向下执行，不打开串口监视器在此句等待

    mpuMode = MODE_SW;                 //设置MPU6050模式，MODE_DMP/MODE_SW
 
    Serial.println(F("Initializing MPU..."));
    mpuReady = mpu.begin(mpuMode);     //初始化MPU6050，并将状态值返回给mpuReady，返回值为0初始化成功        
                                       // 1 = initial memory load failed
                                       // 2 = DMP configuration updates failed
    if (!mpuReady) {                   //mpu初始化失败
        Serial.print(F("MPU Initialization failed!"));
    }
    
#ifdef AXIS_9                          //如果定义了AXIS_9，执行下列，未定义直接跳过
    Serial.println("Initializing HMC5883L...");
    Serial.println(mag.begin() ? "HMC5883L connection successful" : "HMC5883L connection failed");
                                       //mag.begin(); 测试HMC5883L是否连接成功，连接成功返回1，失败返回0
    Serial.println("Calibrate mag start, please roate the sensor in 20s ...");
    mag.calibrateMag(0);               //磁场校准
    Serial.println("Calibrate done.");
    Serial.print("offser:\t");
    Serial.print(mag.xOffset); Serial.print("\t");
    Serial.print(mag.yOffset); Serial.print("\t");
    Serial.println(mag.zOffset);
#endif    
}

void loop() {
    if (!mpuReady) return;            //如果mpu6050初始化失败，返回

#ifdef AXIS_9                         //如果定义了AXIS_9,获取磁场强度
    mag.getMagneto(&mx, &my, &mz);
#endif
    
#ifdef OUTPUT_READABLE_QUATERNION     //如果定义了OUTPUT_READABLE_QUATERNION,返回四元数数据
  #ifdef AXIS_9                       //如果定义了AXIS_9，获取四元数与三轴磁场强度
    mpu.getQuaternion(&q, mx, my, mz);
  #else
    mpu.getQuaternion(&q);            //获取四元数
  #endif                              //串口打印四元数数据
    Serial.print("quat\t");
    Serial.print(q.w);
    Serial.print("\t");
    Serial.print(q.x);
    Serial.print("\t");
    Serial.print(q.y);
    Serial.print("\t");
    Serial.println(q.z);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL    //如果定义了OUTPUT_READABLE_YAWPITCHROL,返回三轴角度数据（偏航、俯仰、滚转）
    // display Euler angles in degrees
  #ifdef AXIS_9                        //如果定义了AXIS_9，获取三轴角度与三轴磁场强度
    mpu.getYawPitchRoll(ypr, mx, my, mz);
  #else
    mpu.getYawPitchRoll(ypr);         //获取三轴角度
  #endif                              //串口打印最终算得的三轴角度值
    Serial.print("ypr\t");
    Serial.print(ypr[0]);            //偏航角度
    Serial.print("\t");
    Serial.print(ypr[1]);            //俯仰角度
    Serial.print("\t");
    Serial.println(ypr[2]);          //滚转角度
#endif
}
