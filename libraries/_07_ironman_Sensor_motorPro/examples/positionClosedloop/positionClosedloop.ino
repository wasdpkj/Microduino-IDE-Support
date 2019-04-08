#include <ironman_Sensor_motorPro.h>

ironmanmotorPro MOTOR(0x03);

#define P_PID_P 15
#define P_PID_I 5
#define P_PID_D 3

#define RATIO        37 //电机齿轮减速比
#define RESOLUTION   24 //速度分辨率 测速箱比例

void setup() {
  Serial.begin(115200);
  MOTOR.begin();

  //设置位置PID
  MOTOR.setP_PID_P(P_PID_P);
  MOTOR.setP_PID_I(P_PID_I);
  MOTOR.setP_PID_D(P_PID_D);

  //获取设置的位置PID
  Serial.println("Start");
  Serial.print(MOTOR.getP_PID_P());
  Serial.print(",");
  Serial.print(MOTOR.getP_PID_I());
  Serial.print(",");
  Serial.println(MOTOR.getP_PID_D());

  MOTOR.setMode(MODE_POSITION);//位置闭环模式
  MOTOR.setRatio(RATIO);//减速比
  MOTOR.setResolution(RESOLUTION);//速度分辨率
  MOTOR.setSpeed(0);
}

void loop() {
  if (MOTOR.available()) {
    //位置环
    MOTOR.setPosition(12346);
    int32_t Position = MOTOR.getSetPosition();
    int32_t measuringPosition = MOTOR.getPosition();
    Serial.print(Position);
    Serial.print(",");
    Serial.print(measuringPosition);

    Serial.println(" ");
    delay(50);
  }
}