#include <Arduino.h>
#include <ironman_Sensor_motorPro.h>
#include <ironman_Sensor_IRremote.h>
ironmanIRremote IRremote;

#define ADDR_MOTORL 0x02
#define ADDR_MOTORR 0x05
#define ADDR_IR     0x04

ironmanmotorPro MOTOR_L(ADDR_MOTORL);
ironmanmotorPro MOTOR_R(ADDR_MOTORR);

#define S_PID_P      6
#define S_PID_I      75
#define S_PID_D      3
#define RATIO        37
#define RESOLUTION   24
#define MAXSPEED     550

void motorProBeginsta(ironmanmotorPro &MOTOR) {
  MOTOR.begin();

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
  MOTOR.setMode(MODE_SPEED);
  MOTOR.setRatio(RATIO);
  MOTOR.setResolution(RESOLUTION);
  MOTOR.setSpeed(0);
  delay(500);
}

void ironmansetSpeed(int16_t speed, ironmanmotorPro &MOTOR)
{
  if (speed > 255 || speed < -255) {
    return;
  }
  if (MOTOR.available ()) {
    if (speed == 0) {
      MOTOR.setSpeed(BRAKE);
    } else {
      MOTOR.setSpeed(map(speed, -255, 255, -MAXSPEED, MAXSPEED));
      Serial.println(MOTOR.getSpeed());
    }
  }
}

void carspeed(int16_t leftspeed, int16_t rightspeed) {
  ironmansetSpeed(leftspeed, MOTOR_L);
  ironmansetSpeed(-rightspeed, MOTOR_R);
}


void setup()
{
  Serial.begin(115200);
  IRremote.begin(ADDR_IR);
  motorProBeginsta(MOTOR_L);
  motorProBeginsta(MOTOR_R);
}

void loop()
{
  //carspeed(200, 200);

  if (IRremote.remotecontrolSta(KEY_UP, PRESSED)) {
    carspeed(200, 200);
    Serial.println("Forward");
  }
  if (IRremote.remotecontrolSta(KEY_DOWN, PRESSED)) {
    carspeed(-200, -200);
    Serial.println("Back");
  }
  if (IRremote.remotecontrolSta(KEY_LEFT, PRESSED)) {
    carspeed(100, 200);
    Serial.println("Left");
  }
  if (IRremote.remotecontrolSta(KEY_RIGHT, PRESSED)) {
    carspeed(200, 100);
    Serial.println("Right");
  }
  if (IRremote.remotecontrolSta(KEY_UP, RELEASED) || IRremote.remotecontrolSta(KEY_DOWN, RELEASED) || IRremote.remotecontrolSta(KEY_LEFT, RELEASED) || IRremote.remotecontrolSta(KEY_RIGHT, RELEASED)) {
    carspeed(0, 0);
    Serial.println("Stop");
  }
 // delay(10);
}
