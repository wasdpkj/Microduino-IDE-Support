#include <Microduino_sensorMotion.h>

sensorMotion mpu6050;

float ypr[3];

void setup()
{
  //串口初始化
  Serial.begin(115200);
  mpu6050.begin();
}

void loop()
{
  Serial.print(mpu6050.getAccelerationRawX());
  Serial.print(" \t");
  Serial.print(mpu6050.getAccelerationRawY());
  Serial.print(" \t");
  Serial.print(mpu6050.getAccelerationRawZ());
  Serial.print(" \t");
  Serial.print(mpu6050.getRotationRawX());
  Serial.print(" \t");
  Serial.print(mpu6050.getRotationRawY());
  Serial.print(" \t");
  Serial.print(mpu6050.getRotationRawZ());
  Serial.print(" \t");
  Serial.print(mpu6050.getYaw());
  Serial.print(" \t");
  Serial.print(mpu6050.getPitch());
  Serial.print(" \t");
  Serial.print(mpu6050.getRoll());
  Serial.println(" ");

//  mpu6050.getYawPitchRoll(ypr);
//  Serial.print("ypr\t");
//  Serial.print(ypr[0]);
//  Serial.print("\t");
//  Serial.print(ypr[1]);
//  Serial.print("\t");
//  Serial.println(ypr[2]);

  delay(10);
}
