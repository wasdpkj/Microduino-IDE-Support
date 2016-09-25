#include <Microduino_Key.h>

Key KeyA(A6, INPUT);
Key KeyB(A6, INPUT);
Key KeyC(A6, INPUT);
Key KeyD(A6, INPUT);
Key KeyE(A6, INPUT);

void setup() {
  Serial.begin(9600);
}

void loop() {
  switch (KeyA.read(680, 720)) {
    case SHORT_PRESS:
      Serial.println("KEY UP(analog) SHORT_PRESS");  //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY UP(analog) LONG_PRESS");    //长按
      break;
  }

  switch (KeyB.read(480, 530)) {
    case SHORT_PRESS:
      Serial.println("KEY LEFT(analog) SHORT_PRESS");  //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY LEFT(analog) LONG_PRESS");    //长按
      break;
  }

  switch (KeyC.read(830, 870)) {
    case SHORT_PRESS:
      Serial.println("KEY RIGHT(analog) SHORT_PRESS");  //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY RIGHT(analog) LONG_PRESS");    //长按
      break;
  }

  switch (KeyD.read(310, 350)) {
    case SHORT_PRESS:
      Serial.println("KEY DOWN(analog) SHORT_PRESS");  //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY DOWN(analog) LONG_PRESS");    //长按
      break;
  }

  switch (KeyE.read(0, 20)) {
    case SHORT_PRESS:
      Serial.println("KEY ENTER(analog) SHORT_PRESS");  //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY ENTER(analog) LONG_PRESS");    //长按
      break;
  }

  delay(15);
}
