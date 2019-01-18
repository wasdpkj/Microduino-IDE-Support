#include "ironmanIRremote.h"
ironmanIRremote IRremote;

uint32_t remotevalue, remotevalueCache;
boolean twoirsta[2];
uint32_t remoteCode[16] = {
  KEY_POWER,
  KEY_UP,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_DOWN,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_OK,
  KEY_VOLP,
  KEY_VOLD,
  KEY_BACK,
  KEY_PLAY,
  KEY_MUTE
};

void remoteSta() {
  if (IRremote.available()) {
    if (IRremote.irrecvAvailable()) {
      remotevalue = IRremote.irrecvValue();
      if (remotevalue > 0) {
        for (uint8_t i = 0; i < 16; i++) {
          if (remotevalue == remoteCode[i]) {
            remotevalueCache = remotevalue;
            twoirsta[0] = true;
          }
        }
      } else {
        twoirsta[1] = true;
      }
    }
  }
}

boolean remotecontrolSta(uint32_t _value, uint8_t mode) {
  remoteSta();
  if (twoirsta[0] && remotevalueCache == _value && mode == 1) {
    twoirsta[0] = false;
    return true;
  } else if (twoirsta[1] && remotevalueCache == _value && mode == 2) {
    twoirsta[1] = false;
    return true;
  }
  return false;
}


void setup() {
  Serial.begin(115200);
  IRremote.begin(0x03);
  Serial.print("Start");
}

void loop() {

  if (remotecontrolSta(KEY_POWER, 1)) {
    Serial.println("Power Pressed");
  }
  if (remotecontrolSta(KEY_POWER, 2)) {
    Serial.println("Power Released");
  }

  if (remotecontrolSta(KEY_A, 1)) {
    Serial.println("A Pressed");
  }
  if (remotecontrolSta(KEY_A, 2)) {
    Serial.println("A Released");
  }

}
