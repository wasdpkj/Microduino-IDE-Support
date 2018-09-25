#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

uint8_t datacolor[6];

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  car.getColorRaw(datacolor);
  for (int8_t i = 0; i < 6; i++) {
    Serial.print(datacolor[i]);
    Serial.print(" , ");
  }
  Serial.print("\n");
}
