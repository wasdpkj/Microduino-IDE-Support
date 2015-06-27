// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

int motorDirPin = 5;
int motorStepPin = 7;
int motorEnPin = 8;

int motor1DirPin = 18;
int motor1StepPin = 22;
int motor1EnPin = 23;

// Define a stepper and the pins it will use
AccelStepper stepper1(1, motorStepPin, motorDirPin);
AccelStepper stepper2(1, motor1StepPin, motor1DirPin);
//A4982,left to right(OUT2B OUT2A OUT1A OUT1B):red,green,yellow,blue

void setup()
{  
  stepper1.setMaxSpeed(200.0);
  stepper1.setAcceleration(100.0);
  stepper1.moveTo(24);

  stepper2.setMaxSpeed(300.0);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(1000000);
}

void loop()
{
  // Change direction at the limits
  if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());
  stepper1.run();
  stepper2.run();
}