#include <BittyBuggy.h>

#define mySerial Serial1  // Core+ D2,D3
BittyBuggy car(&mySerial);

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  /*
    if (!digitalRead(6)) {
    car.tone(600, 1000);
    delay(300);
    }
  */
  car.tone(600);
  delay(500);
  car.noTone();
  delay(500);
}
