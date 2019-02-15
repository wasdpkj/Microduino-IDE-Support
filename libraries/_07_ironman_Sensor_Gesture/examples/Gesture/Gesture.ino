#include <ironman_Sensor_Gesture.h>
ironmanGesture Gesture;

void setup() {
  Serial.begin(115200);
  Gesture.begin(0x03);
  Gesture.setMode(MODE_Gesture);
  Serial.println("Start");
}

void loop() {
  if (Gesture.available()) {
    switch (Gesture.readGesture()) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      default:
        ;
    }
    delay(100);
  }
}
