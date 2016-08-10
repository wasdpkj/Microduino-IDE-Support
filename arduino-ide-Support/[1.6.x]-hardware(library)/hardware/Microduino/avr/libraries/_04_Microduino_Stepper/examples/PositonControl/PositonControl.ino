#include <Microduino_Stepper.h>

StepServo stepper(PIN_DIRA, PIN_STEPA);

void setup() {
  stepper.begin();
  stepper.setMaxSpeed(1960);
  stepper.setSpeed(1960);
  stepper.setAcceleration(8000);
  stepper.moveTo(320); //move 32000 steps (should be 10 rev)
}

void loop() {
  //if stepper is at desired location
  if (stepper.distanceToGo() == 0) {
    //go the other way the same amount of steps
    //so if current position is 400 steps out, go position -400
    stepper.moveTo(-stepper.getCurrentPosition());
  }

  //these must be called as often as possible to ensure smooth operation
  //any delay will cause jerky motion
  stepper.run();
}