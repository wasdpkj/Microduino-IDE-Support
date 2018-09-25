#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

void setup()
{
  Serial.begin(57600);
  while (!car.begin())
    ;
  car.setColorLEDA(0, 50, 50);
  car.setColorLEDB(0, 50, 50);
  delay(1000);
  car.setColorLEDA(0x000000);
  car.setColorLEDB(0x000000);
}

void loop()
{
  car.setAllLED(255, 0, 0);
  delay(500);
  car.setAllLED(COLOR_GREEN);
  delay(500);
  car.setAllLED(COLOR_BLUE);
  delay(500);
}