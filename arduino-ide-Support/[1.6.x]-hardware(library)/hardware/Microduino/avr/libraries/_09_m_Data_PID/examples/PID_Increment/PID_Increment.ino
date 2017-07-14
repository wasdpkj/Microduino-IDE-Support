/********************************************************
   PID Basic Example
   Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <Microduino_PID.h>

#define PIN_INPUT 0
#define PIN_OUTPUT 3

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp = 2, Ki = 5, Kd = 1;
PID myPID(Kp, Ki, Kd);

void setup()
{
  Serial.begin(115200);

  //initialize the variables we're linked to
  Input = analogRead(PIN_INPUT);
  Setpoint = 100;

  Serial.println("IncrementPID controler started ！");
  myPID.setSampleTime(10);
}

void loop()
{
  Input = analogRead(PIN_INPUT);
  Output = myPID.IncrementCompute(Input, Setpoint);
  analogWrite(PIN_OUTPUT, (int16_t)Output);
}