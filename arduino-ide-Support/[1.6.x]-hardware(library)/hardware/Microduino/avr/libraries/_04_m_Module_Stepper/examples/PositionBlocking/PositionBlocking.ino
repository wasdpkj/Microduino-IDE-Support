#include <Microduino_Stepper.h>

StepServo stepper(PIN_DIRA, PIN_STEPA);

void setup()
{  
    stepper.begin();
    stepper.setMaxSpeed(200.0);
    stepper.setAcceleration(100.0);
}

void loop()
{    
    stepper.runToNewPosition(0);
    stepper.runToNewPosition(500);
    stepper.runToNewPosition(100);
    stepper.runToNewPosition(120);
}