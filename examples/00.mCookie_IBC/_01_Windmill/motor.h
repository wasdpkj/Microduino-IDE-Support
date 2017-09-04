#include <Microduino_Motor.h>

Motor MotorLeft(MOTOR0_PINA, MOTOR0_PINB);
Motor MotorRight(MOTOR1_PINA, MOTOR1_PINB);

void motorRun(int left_speed, int right_speed)
{
  MotorLeft.setSpeed(left_speed);
  MotorRight.setSpeed(right_speed);
}

//Stops the motors.
void motorBrake() {
  MotorLeft.Brake();
  MotorRight.Brake();
}
//Turns off the motors. Different from motorBrake() because this saves battery.
void motorFree() {
  MotorLeft.setSpeed(FREE);
  MotorRight.setSpeed(FREE);
}
