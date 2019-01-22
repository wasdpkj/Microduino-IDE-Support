#include <ironman_Sensor_IRremote.h>
ironmanIRremote IRremote;

void setup() {
  Serial.begin(115200);
  IRremote.begin(0x03);
  Serial.print("Start");
}

void loop() {

  if (IRremote.remotecontrolSta(KEY_POWER, PRESSED)) {
    Serial.println("Power Pressed");
  }
  if (IRremote.remotecontrolSta(KEY_POWER, RELEASED)) {
    Serial.println("Power Released");
  }

  if (IRremote.remotecontrolSta(KEY_A, PRESSED)) {
    Serial.println("A Pressed");
  }
  if (IRremote.remotecontrolSta(KEY_A, RELEASED)) {
    Serial.println("A Released");
  }

}
