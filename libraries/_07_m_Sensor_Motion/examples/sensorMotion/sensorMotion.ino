/*
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  Microduino wiki:
  http://wiki.microduino.cn

  本库可支持Sensor-Motion、MMA7660两种姿态传感器
  本库自动识别I2C地址来识别传感器类型
  当使用MMA7760时，仅可获取三轴加速度数据，其他数据不可用
  获取Sensor-Motion的温度，串口监视器打印数据。并且检测传感器是否在线。

  电路：
    接到核心的IIC引脚

  2019年3月13日修改
  By MX,CWJ,PKJ
*/

#include <Microduino_sensorMotion.h>

sensorMotion motion;

float ax, ay, az;
float gx, gy, gz;

void setup() {
  //串口初始化
  Serial.begin(115200);
  motion.begin();
  Serial.print("Version:");
  Serial.println(motion.getVersion());
  switch (motion.getSensorType()) {
    case SENSOR_TYPE_SENSORMOTION:
      Serial.println("Type:Sensor Motion");
      break;
    case SENSOR_TYPE_MMA7660:
      Serial.println("Type:MMA7660");
      break;
    case SENSOR_TYPE_UNKNOW:
      Serial.println("Type:ERROR Unknow");
      break;
  }
}

void loop() {
  Serial.print(motion.getAccelerationRawX());
  Serial.print(" \t");
  Serial.print(motion.getAccelerationRawY());
  Serial.print(" \t");
  Serial.print(motion.getAccelerationRawZ());
  Serial.print(" \t");
  Serial.print(motion.getRotationRawX());
  Serial.print(" \t");
  Serial.print(motion.getRotationRawY());
  Serial.print(" \t");
  Serial.print(motion.getRotationRawZ());
  Serial.print(" \t");
  Serial.print(motion.getYaw());
  Serial.print(" \t");
  Serial.print(motion.getPitch());
  Serial.print(" \t");
  Serial.print(motion.getRoll());
  Serial.println(" ");

  //  motion.getAccelerationRaw(&ax, &ay, &az);
  //  Serial.print("a/g:\t");
  //  Serial.print(ax); Serial.print("\t");
  //  Serial.print(ay); Serial.print("\t");
  //  Serial.println(az);

  //  motion.getMotionRaw6(&ax, &ay, &az, &gx, &gy, &gz);
  //  Serial.print("a/g:\t");
  //  Serial.print(ax); Serial.print("\t");
  //  Serial.print(ay); Serial.print("\t");
  //  Serial.print(az); Serial.print("\t");
  //  Serial.print(gx); Serial.print("\t");
  //  Serial.print(gy); Serial.print("\t");
  //  Serial.println(gz);

  delay(50);
}