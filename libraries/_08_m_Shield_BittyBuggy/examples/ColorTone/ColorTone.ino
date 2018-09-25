#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

uint16_t tonelist[8] = {523, 587, 659, 697, 784, 880, 988, 0};

uint8_t dataspecialcolor[2];

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  //  dataspecialcolor[0]=car.getColorA();
  //  dataspecialcolor[1]=car.getColorB();

  car.getColor(dataspecialcolor);
  Serial.print(dataspecialcolor[0]);
  Serial.print(" , ");
  Serial.println(dataspecialcolor[1]);

  if (dataspecialcolor[0] > 0 && dataspecialcolor[0] < 8 && (dataspecialcolor[0] == dataspecialcolor[1])) {
    car.tone(tonelist[dataspecialcolor[0] - 1]);
  } else {
    car.noTone();
  }
}