
#include "MPU6050_6Axis_Microduino.h"

MPU6050 mpu;

//#define OUTPUT_READABLE_QUATERNION   //如果定义这句，可查询实际的四元数数据[w, x, y, z]
#define OUTPUT_READABLE_YAWPITCHROLL   //如果定义这句，得到的是由四元数算得的偏航、俯仰、滚转的角度值

Quaternion q;                          // [w, x, y, z]         四元数
float ypr[3];                          // [yaw, pitch, roll]   yaw/pitch/roll（偏航、俯仰、滚转）的角度值
uint8_t mpuMode;                       //MPU6050模式
bool mpuReady;                         //mpu初始化状态值

void setup() {
    Serial.begin(115200);             //串口初始化，波特率为115200
    while (!Serial);                  //打开串口监视器向下执行，不打开串口监视器在此句等待

    mpuMode = MODE_DMP;               //设置MPU6050模式，MODE_DMP/MODE_SW

    Serial.println(F("Initializing MPU..."));
    mpuReady = mpu.begin(mpuMode);   //初始化MPU6050，并将状态值返回给mpuReady，返回值为0初始化成功        
                                     // 1 = initial memory load failed
                                     // 2 = DMP configuration updates failed
    if (!mpuReady) {                 //mpu初始化失败
        Serial.print(F("MPU Initialization failed!"));
    }
}

void loop() {
    if (!mpuReady) return;           //如果mpu6050初始化失败，返回
    
#ifdef OUTPUT_READABLE_QUATERNION    //如果定义了OUTPUT_READABLE_QUATERNION,返回四元数数据
    mpu.getQuaternion(&q);           //获取四元数
    Serial.print("quat\t");
    Serial.print(q.w);
    Serial.print("\t");
    Serial.print(q.x);
    Serial.print("\t");
    Serial.print(q.y);
    Serial.print("\t");
    Serial.println(q.z);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL  //如果定义了OUTPUT_READABLE_YAWPITCHROL,返回三轴角度数据（偏航、俯仰、滚转）
    mpu.getYawPitchRoll(ypr);        //获取三轴角度
    Serial.print("ypr\t");
    Serial.print(ypr[0]);           //偏航角度
    Serial.print("\t");
    Serial.print(ypr[1]);           //俯仰角度
    Serial.print("\t");
    Serial.println(ypr[2]);         //滚转角度
#endif
}
