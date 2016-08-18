#include <Microduino_Stepper.h>

StepServo stepperA(PIN_DIRA, PIN_STEPA);
StepServo stepperB(PIN_DIRB, PIN_STEPB);

void setup()
{
  
  stepperA.begin();
  stepperA.setMaxSpeed(10000.0);
  stepperA.setAcceleration(100.0);
  stepperA.moveTo(3200);

  stepperB.begin();
  stepperB.setMaxSpeed(10000.0);
  stepperB.setAcceleration(100.0);
  stepperB.moveTo(3200);
}

void loop()
{
  // Change direction at the limits
  if (stepperA.distanceToGo() == 0)
    stepperA.moveTo(-stepperA.getCurrentPosition());
  stepperA.run();

  if (stepperB.distanceToGo() == 0)
    stepperB.moveTo(-stepperB.getCurrentPosition());
  stepperB.run();
}