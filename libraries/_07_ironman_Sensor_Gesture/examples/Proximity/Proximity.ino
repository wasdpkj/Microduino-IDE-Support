#include <ironman_Sensor_Gesture.h>
irconmanGesture Gesture;

void setup() {
  Serial.begin(115200);
  Gesture.begin(0x03);
  Gesture.setMode(MODE_Proximity);
  Serial.println("Start");
}

void loop() {
  if (Gesture.available ()) {
    Serial.println(Gesture.readProximity());
    delay(100);
  }
}
