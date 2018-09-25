#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

uint8_t dataline[2];
int16_t carspeed[2];

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  //car.getLineAlone(dataline, RED, GREEN);
  //car.getLineA(RED);
  //car.getLineB(GREEN);

  car.getLine(dataline, GREEN);
  trackControl(dataline[0], dataline[1]);
  car.setSpeed(carspeed[0], carspeed[1]);
  Serial.print("A:");
  Serial.print(dataline[0]);
  Serial.print("  B:");
  Serial.println(dataline[1]);
}

void trackControl(uint8_t trackVal1, uint8_t trackVal2)
{
  if (trackVal1 < 135 && trackVal2 < 135) {
    carspeed[0] = 0;
    carspeed[1] = 0;
  }
  else {
    carspeed[0] = 220 + (trackVal1 - 240) * 2; //白248 黑 130
    carspeed[1] = 220 + (trackVal2 - 240) * 2;
  }
}