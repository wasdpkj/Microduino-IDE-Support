#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

void setup()
{
  Serial.begin(57600);
  while (!car.begin())
    ;
  car.setSpeedA(0);
  car.setSpeedB(0);
}

void loop()
{
  Serial.println("Forward!");
  car.setSpeed(50, 50);
  delay(2000);
  Serial.println("BRAKE!");
  car.setSpeed(BRAKE, BRAKE);
  delay(2000);
  Serial.println("Back!");
  car.setSpeed(-250, -250);
  delay(2000);
  Serial.println("Free!");
  car.setSpeed(FREE, FREE);
  delay(2000);
}