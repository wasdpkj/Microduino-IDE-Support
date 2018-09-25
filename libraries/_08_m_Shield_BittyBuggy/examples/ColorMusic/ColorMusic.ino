#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

#define SPEEN 10

uint16_t tonelist[8] = {523, 587, 659, 697, 784, 880, 0, 988};

uint8_t dataspecialcolor[2];
int16_t carspeed[2];

uint8_t turnNum;
boolean turnSta;

void setup() {
  Serial.begin(115200);
  while (!car.begin())
    ;
  while (dataspecialcolor[0] != 8 && dataspecialcolor[1] != 8) {
    car.getColor(dataspecialcolor);
    car.setSpeed(0, 0);
  }
}

void loop() {
  //  dataspecialcolor[0]=car.getColorA();
  //  dataspecialcolor[1]=car.getColorB();

  car.getColor(dataspecialcolor);
  //  Serial.print(dataspecialcolor[0]);
  //  Serial.print(" , ");
  //  Serial.println(dataspecialcolor[1]);
  trackControl(dataspecialcolor[0], dataspecialcolor[1]);
  car.setSpeed(carspeed[0], carspeed[1]);

  if (dataspecialcolor[1] > 0 && dataspecialcolor[1] < 7) {
    car.tone(tonelist[dataspecialcolor[1] - 1]);
  } else {
    car.noTone();
  }

}

void trackControl(uint8_t trackVal1, uint8_t trackVal2) {
  if (trackVal1 == 7 && trackVal2 != 7) {
    carspeed[0] = 0;
    carspeed[1] = SPEEN;
    turnSta = false;
  } else  if (trackVal2 == 7 && trackVal1 != 7) {
    carspeed[0] = SPEEN;
    carspeed[1] = 0;
    turnSta = false;
  } else if (trackVal2 == 7 && trackVal1 == 7) {
    if (!turnSta) {
      turnNum++;
      turnSta = true;
    }
    if (turnNum > 4) {
      turnNum = 1;
    }
    if (turnNum == 1 || turnNum == 4) {//左
      car.setSpeed(0, SPEEN);
      car.tone(600, 100);
    } else {//右
      car.setSpeed(SPEEN, 0);
      car.tone(1200, 100);
    }
    delay(200);
  } else {
    carspeed[0] = SPEEN;
    carspeed[1] = SPEEN;
    turnSta = false;
  }
}