#include <Microduino_Key.h>

DigitalKey keyA0(A0);
AnalogKey keyA2(A2);

void setup() {
  Serial.begin(9600);
  keyA0.begin(INPUT_PULLUP);
  keyA2.begin(INPUT);
}

void loop() {
  switch (keyA0.readEvent()) {
    case SHORT_PRESS:
      Serial.println("KEY A0(digital) SHORT_PRESS"); 	//短按
      break;
    case LONG_PRESS:
      Serial.println("KEY A0(digital) LONG_PRESS");    //长按
      break;
  }

  switch (keyA2.readEvent(0, 100)) {
    case SHORT_PRESS:
      Serial.println("KEY A2(analog) SHORT_PRESS"); 	//短按
      break;
    case LONG_PRESS:
      Serial.println("KEY A2(analog) LONG_PRESS");    //长按
      break;
  }
  delay(15);
}