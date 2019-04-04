#include <ironman_Sensor_motorPro.h>

ironmanmotorPro MOTOR(0x03);

//1:48电机的PID参数
#define S_PID_P 6
#define S_PID_I 75
#define S_PID_D 3

#define RATIO        37 //电机齿轮减速比
#define RESOLUTION   24 //速度分辨率 测速箱比例

void setup() {
  Serial.begin(115200);
  MOTOR.begin();

  //设置速度PID
  MOTOR.setS_PID_P(S_PID_P);
  MOTOR.setS_PID_I(S_PID_I);
  MOTOR.setS_PID_D(S_PID_D);
  
  Serial.println("Start");
  //获取设置的速度PID
  Serial.print(MOTOR.getS_PID_P());
  Serial.print(",");
  Serial.print(MOTOR.getS_PID_I());
  Serial.print(",");
  Serial.println(MOTOR.getS_PID_D());

  MOTOR.setMode(MODE_SPEED);//速度闭环模式
  MOTOR.setRatio(RATIO);//减速比
  MOTOR.setResolution(RESOLUTION);//速度分辨率
  MOTOR.setSpeed(0);
}

void loop() {
  if (MOTOR.available ()) {
    MOTOR.setSpeed(255);//-255 - 255
    int16_t setspeed = MOTOR.getSetSpeed();//设置的速度
    int16_t measuringspeed = MOTOR.getSpeed();//测量速度
    Serial.print(setspeed);
    Serial.print(",");
    Serial.println(measuringspeed);

    delay(50);
  }
}