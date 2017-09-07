// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

#include "Microduino_Audio.h"

#if defined (ESP32)
Audio::Audio(HardwareSerial *ser, int _rx, int _tx) {
  common_init();
  audioHwSerial = ser;
  pinRX = _rx;
  pinTX = _tx;
}
#elif defined (__AVR__)
Audio::Audio(HardwareSerial *ser) {
  common_init();
  audioHwSerial = ser;
}

Audio::Audio(SoftwareSerial *ser) {
  common_init();
  audioSwSerial = ser;
}
#endif


void Audio::common_init(void) {
  audioHwSerial = NULL;
#if defined (__AVR__)
  audioSwSerial = NULL;
#endif
}

void Audio::begin(uint8_t device, uint8_t mode, uint8_t vol) {
#if defined (ESP32)
  audioHwSerial->begin(9600, SERIAL_8N1, pinRX, pinTX);
#elif defined (__AVR__)
  if (audioSwSerial)
    audioSwSerial->begin(9600);
  else
    audioHwSerial->begin(9600);
#endif
  reset();
  setDevice(device);
  setMode(mode);
  setVolume(vol);
}


void Audio::sendCommand(uint8_t cmd, uint8_t *buf, uint16_t len) {
  sendBuffer[0] = STX;
  sendBuffer[1] = len + 2;
  sendBuffer[2] = cmd;
  if (len > 0)
    memcpy(sendBuffer + 3, buf, len);
  sendBuffer[len + 3] = ETX;
  if (audioHwSerial)
    audioHwSerial->write(sendBuffer, len + 4);
#if defined (__AVR__)
  else
    audioSwSerial->write(sendBuffer, len + 4);
#endif

  delay(160);
}

void Audio::nextMusic() {
  sendCommand(CMD_NEXT, NULL, 0);
}

void Audio::prevMusic() {
  sendCommand(CMD_PREV, NULL, 0);
}

void Audio::chooseMusic(uint16_t num) {
  uint8_t buffer[2];
  buffer[0] = num >> 8;
  buffer[1] = num & 0xFF;
  sendCommand(CMD_CHOOSE, buffer, 2);
}

void Audio::volumeUp() {
  sendCommand(CMD_UP, NULL, 0);
}

void Audio::volumeDown() {
  sendCommand(CMD_DOWN, NULL, 0);
}

void Audio::setVolume(uint8_t vol) {
  sendCommand(CMD_VOL, &vol, 1);
}

void Audio::setEq(uint8_t eq) {
  sendCommand(CMD_EQ, &eq, 1);
}

void Audio::setDevice(uint8_t device) {
  sendCommand(CMD_DEVICE, &device, 1);
  delay(1500);
}

void Audio::sleep() {
  sendCommand(CMD_SLEEP, NULL, 0);
}

void Audio::reset() {
  sendCommand(CMD_RESET, NULL, 0);
  delay(1000);
}

void Audio::playMusic() {
  sendCommand(CMD_PLAY, NULL, 0);
}

void Audio::pauseMusic() {
  sendCommand(CMD_PAUSE, NULL, 0);
}

void Audio::folder(uint8_t temp) {
  sendCommand(CMD_FOLDER, &temp, 1);
}

void Audio::setMode(uint8_t temp) {
  sendCommand(CMD_MODE, &temp, 1);
}

void Audio::chooseFile(uint8_t folder, uint8_t file) {
  uint8_t buffer[2];
  buffer[0] = folder;
  buffer[1] = file;
  sendCommand(CMD_FILE, buffer, 2);
}

uint16_t Audio::dataParse() {
  uint16_t sum = 0;
  uint8_t temp;
  if (audioHwSerial) {
    while (audioHwSerial->available()) {
      temp = char(audioHwSerial->read());
      if (temp > 47 && temp < 58) {
        temp -= 48;
      } else if (temp > 96 && temp < 103) {
        temp -= 87;
      }
      sum = sum * 16 + temp;
      delay(1);
    }
  }
#if defined (__AVR__)
  else {
    while (audioSwSerial->available()) {
      temp = char(audioSwSerial->read());
      if (temp > 47 && temp < 58) {
        temp -= 48;
      } else if (temp > 96 && temp < 103) {
        temp -= 87;
      }
      sum = sum * 16 + temp;
      delay(1);
    }
  }
#endif
  return sum;
}

uint16_t Audio::queryNum(uint8_t cmd) {
  if (audioHwSerial) {
    audioHwSerial->flush();
  }
#if defined (__AVR__)
  else {
    audioSwSerial->stopListening();
    audioSwSerial->listen();
  }
#endif
  sendCommand(cmd, NULL, 0);
  return dataParse();
}

uint16_t Audio::queryTF() {
  return queryNum(QUERY_TF);
}

uint16_t Audio::queryTFFile() {
  return queryNum(QUERY_TF_FILE);
}

uint16_t Audio::queryFlash() {
  return queryNum(QUERY_FLASH);
}

uint16_t Audio::queryFlashFile() {
  return queryNum(QUERY_FLASH_FILE);
}

uint16_t Audio::queryTotalTime() {
  return queryNum(QUERY_TTIME);
}

uint16_t Audio::queryPlayTime() {
  return queryNum(QUERY_PTIME);
}

String Audio::queryName() {
  String nameCache = "";
  char temp;
  uint16_t i = 0;
  if (audioHwSerial) {
    audioHwSerial->flush();
  }
#if defined (__AVR__)
  else {
    audioSwSerial->stopListening();
    audioSwSerial->listen();
  }
#endif
  sendCommand(QUERY_NAME, NULL, 0);
  if (audioHwSerial) {
    while (audioHwSerial->available()) {
      temp = audioHwSerial->read();
      nameCache += temp;
      if (temp == 32) {
        i++;
      } else {
        i = 0;
      }
      if (i >= 3) {
        i = 0;
        nameCache.replace("   ", ".");
      }
      delay(1);
    }
  }
#if defined (__AVR__)
  else {
    while (audioSwSerial->available()) {
      temp = audioSwSerial->read();
      nameCache += temp;
      if (temp == 32) {
        i++;
      } else {
        i = 0;
      }
      if (i >= 3) {
        i = 0;
        nameCache.replace("   ", ".");
      }
      delay(1);
    }
  }
#endif
  return nameCache;
}

//
// END OF FILE
//
