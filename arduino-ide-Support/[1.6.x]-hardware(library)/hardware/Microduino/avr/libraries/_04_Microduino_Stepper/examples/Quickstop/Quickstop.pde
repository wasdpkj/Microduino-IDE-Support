#include <Microduino_Stepper.h>

StepServo stepper(PIN_DIRA, PIN_STEPA);

void setup()
{  
  stepper.begin();
  stepper.setMaxSpeed(150);
  stepper.setAcceleration(100);
}

void loop()
{    
  stepper.moveTo(500);
  while (stepper.getCurrentPosition() != 300) // Full speed up to 300
    stepper.run();
  stepper.stop(); // Stop as fast as possible: sets new target
  stepper.runToPosition(); 
  // Now stopped after quickstop

  // Now go backwards
  stepper.moveTo(-500);
  while (stepper.getCurrentPosition() != 0) // Full speed basck to 0
    stepper.run();
  stepper.stop(); // Stop as fast as possible: sets new target
  stepper.runToPosition(); 
  // Now stopped after quickstop
}