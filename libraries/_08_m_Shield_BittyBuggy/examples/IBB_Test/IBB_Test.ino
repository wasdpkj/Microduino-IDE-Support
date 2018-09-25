#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

void setup()
{
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop()
{
  car.setAllLED(255, 0, 0);
  car.setSpeed(0, 0);
  car.tone(500);
  delay(500);
  car.setAllLED(COLOR_GREEN);
  car.setSpeed(255, 255);
  car.tone(800);
  delay(500);
  car.setAllLED(COLOR_BLUE);
  car.setSpeed(-255, -255);
  car.tone(1200);
  delay(500);
}
