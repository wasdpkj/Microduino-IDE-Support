#include <ironman_shield_coreNrf.h>

ironmanShieldNrf shieldNrf;

void setup() {
  Serial.begin(115200);
  Serial.println(shieldNrf.begin());
  Serial.println("Start");
}

void loop() {
  if (shieldNrf.shieldbuttonSta(KEY_X, PRESSED)) {
    Serial.println("X Pressed");
  }
  if (shieldNrf.shieldbuttonSta(KEY_X, RELEASED)) {
    Serial.println("X Released");
  }

  if (shieldNrf.shieldbuttonSta(KEY_Y, PRESSED)) {
    Serial.println("Y Pressed");
  }
  if (shieldNrf.shieldbuttonSta(KEY_Y, RELEASED)) {
    Serial.println("Y Released");
  }
}