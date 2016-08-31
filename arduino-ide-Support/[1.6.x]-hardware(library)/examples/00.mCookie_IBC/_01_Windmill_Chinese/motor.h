#include <Microduino_Motor.h>
#if defined(__AVR_ATmega32U4__) || (__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define MOTOR_PIN0A 8
#define MOTOR_PIN0B 6
#define MOTOR_PIN1A 7
#define MOTOR_PIN1B 5
#else
#define MOTOR_PIN0A 6
#define MOTOR_PIN0B 8
#define MOTOR_PIN1A 5
#define MOTOR_PIN1B 7
#endif
Motor MotorLeft(MOTOR_PIN0A, MOTOR_PIN0B);
Motor MotorRight(MOTOR_PIN1A, MOTOR_PIN1B);

unsigned long motor_timer;

void motorRun(int left_speed, int right_speed)
{
  MotorLeft.Driver(left_speed);
  MotorRight.Driver(right_speed);
}

//Stops the motors.
void motorBrake() {
  MotorLeft.Brake();
  MotorRight.Brake();
}
//Turns off the motors. Different from motorBrake() because this saves battery.
void motorFree() {
  MotorLeft.Free();
  MotorRight.Free();
}

