#include <Microduino_Key.h>

AnalogKey keyU(A2);
AnalogKey keyD(A2);
AnalogKey keyL(A2);
AnalogKey keyR(A2);
AnalogKey keyP(A2);

void setup() {
  Serial.begin(9600);
}

void loop() {

  switch (keyU.readEvent(650, 820)) {
    case SHORT_PRESS:
      Serial.println("KEY UP(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY UP(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyD.readEvent(200, 450)) {
    case SHORT_PRESS:
      Serial.println("KEY DOWN(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY DOWN(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyL.readEvent(450, 650)) {
    case SHORT_PRESS:
      Serial.println("KEY LEFT(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY LEFT(analog) LONG_PRESS");    //长按
      break;
  }

  switch (keyR.readEvent(820, 980)) {
    case SHORT_PRESS:
      Serial.println("KEY RIGHT(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY RIGHT(analog) LONG_PRESS");    //长按
      break;
  }

    switch (keyP.readEvent(0, 100)) {
    case SHORT_PRESS:
      Serial.println("KEY PRESS(analog) SHORT_PRESS");   //短按
      break;
    case LONG_PRESS:
      Serial.println("KEY PRESS(analog) LONG_PRESS");    //长按
      break;
  }

  delay(15);
}
