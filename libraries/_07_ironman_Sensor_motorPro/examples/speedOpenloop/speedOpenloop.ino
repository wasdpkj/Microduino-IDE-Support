#include <ironman_Sensor_motorPro.h>

ironmanmotorPro MOTOR(0x03);

#define RATIO        37 //电机齿轮减速比
#define RESOLUTION   24 //速度分辨率 测速箱比例

void setup() {
  Serial.begin(115200);
  MOTOR.begin();
  MOTOR.setMode(MODE_OPEN); //开环
  MOTOR.setRatio(RATIO);//减速比
  MOTOR.setResolution(RESOLUTION);//速度分辨率
  MOTOR.setSpeed(0);
}

void loop() {
  if (MOTOR.available ()) {
    Serial.println("Welcome to AVR");
   
    MOTOR.setSpeed(255);
    Serial.println(MOTOR.getSetSpeed());
    delay(2000);
    MOTOR.setSpeed(0);
    Serial.println(MOTOR.getSetSpeed());
    delay(1000);
    MOTOR.setSpeed(-255);
    Serial.println(MOTOR.getSetSpeed());
    delay(2000);
    MOTOR.setSpeed(0);
    Serial.println(MOTOR.getSetSpeed());
    delay(1000);
  }
}