#include "Microduino_MicroRobot.h"
#include "I2Cdev.h"

MicroRobot::MicroRobot() {

}

//***********************PMU*********************************
float MicroRobot::getVoltage() {
  uint16_t batRaw = 0;
  I2Cdev::readWord(I2C_ADDR_PMU, ADDR16_BAT * 2, &batRaw);
  return batRaw * 5.0 * 2.01 / 1024.0;
}

//***********************servo*********************************
void MicroRobot::servoWrite(uint8_t index, int16_t value) {
  if (index > MAX_SERVOS - 1)
    return;
  uint8_t buf[2];
  uint16_t _value = constrain(value, 0, 180);
  _value = map(_value, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  buf[0] = _value >> 8;
  buf[1] = _value & 0xFF;
  I2Cdev::writeBytes(I2C_ADDR_PMU, (ADDR16_SERVO + 4 - index) * 2, 2, buf);
}

void MicroRobot::servoWriteRaw(uint8_t index, int16_t value) {
  if (index > MAX_SERVOS - 1)
    return;

  uint8_t buf[2];
  uint16_t _value = constrain(value, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  buf[0] = _value >> 8;
  buf[1] = _value & 0xFF;
  I2Cdev::writeBytes(I2C_ADDR_PMU, (ADDR16_SERVO + 4 - index) * 2, 2, buf);
}

uint16_t MicroRobot::servoRead(uint8_t index) {
  uint16_t buf;
  if (index > MAX_SERVOS - 1)
    return 0;

  I2Cdev::readWord(I2C_ADDR_PMU, (ADDR16_SERVO + 4 - index) * 2, &buf);
  buf = map(buf, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 0, 180);
  return buf;
}

uint16_t MicroRobot::servoReadRaw(uint8_t index) {
  if (index > MAX_SERVOS - 1)
    return 0;

  uint16_t buf;
  I2Cdev::readWord(I2C_ADDR_PMU, (ADDR16_SERVO + 4 - index) * 2, &buf);
  return buf;
}

void MicroRobot::digitalWrite(uint8_t index, uint8_t value) {
  if (index > MAX_SERVOS - 1)
    return;

  uint8_t buf[2];
  if (value == LOW) {
    buf[0] = 0;
    buf[1] = 0;
  } else {
    buf[0] = OUTPUT_HIGH >> 8;
    buf[1] = OUTPUT_HIGH & 0xFF;
  }
  I2Cdev::writeBytes(I2C_ADDR_PMU, (ADDR16_SERVO + 4 - index) * 2, 2, buf);
}


//**************************motor******************************

uint8_t MicroRobot::motorInit(uint8_t index, uint8_t _bit) {
  bool boolFlag;
  _bit = constrain(_bit, 8, 14);
  Multiple[index - 1] = 0X4000 >> _bit;
  Wire.beginTransmission(MotorAddress[index - 1]);
  boolFlag = Wire.endTransmission();
  reset(index,2);
  return boolFlag;
}

void MicroRobot::setRatio(uint8_t index, int16_t _ratio) {
  if ((index < 1) || (index > 4))  return;
  ratio[index - 1] = _ratio;
}

void MicroRobot::setResolution(uint8_t index, uint8_t _resolution) {
  if ((index < 1) || (index > 4))  return;
  resolution[index - 1] = _resolution;
}

void MicroRobot::reset(uint8_t index, uint8_t _reset) {
  if ((index < 1) || (index > 4))  return;
  write8(index, ADDR8_RESET, &_reset, 1);
}

void MicroRobot::setMode(uint8_t index, uint8_t _mode) {
  if ((index < 1) || (index > 4))  return;
  motorMode[index-1] = _mode;
  write8(index, ADDR8_MODE, &_mode, 1);
}

void MicroRobot::setS_PID(uint8_t index, float p, float i, float d) {
  if ((index < 1) || (index > 4))  return;
  float PIDBuf[3];
  PIDBuf[0] = p;
  PIDBuf[1] = i;
  PIDBuf[2] = d;
  write32(index, ADDR32_SPD_KP, PIDBuf, 3);
}

void MicroRobot::setS_PID_P(uint8_t index, float p) {
  if ((index < 1) || (index > 4))  return;
  write32(index, ADDR32_SPD_KP, &p, 1);
}

void MicroRobot::setS_PID_I(uint8_t index, float i) {
  if ((index < 1) || (index > 4))  return;
  write32(index, ADDR32_SPD_KI, &i, 1);
}

void MicroRobot::setS_PID_D(uint8_t index, float d) {
  if ((index < 1) || (index > 4))  return;
  write32(index, ADDR32_SPD_KD, &d, 1);
}

void MicroRobot::setP_PID(uint8_t index, float p, float i, float d) {
  if ((index < 1) || (index > 4))  return;
  float PIDBuf[3];
  PIDBuf[0] = p;
  PIDBuf[1] = i;
  PIDBuf[2] = d;
  write32(index, ADDR32_PST_KP, PIDBuf, 3);
}

void MicroRobot::setP_PID_P(uint8_t index, float p) {
  if ((index < 1) || (index > 4))  return;
  write32(index, ADDR32_PST_KP, &p, 1);
}

void MicroRobot::setP_PID_I(uint8_t index, float i) {
  if ((index < 1) || (index > 4))  return;
  write32(index, ADDR32_PST_KI, &i, 1);
}

void MicroRobot::setP_PID_D(uint8_t index, float d) {
  if ((index < 1) || (index > 4))  return;
  write32(index, ADDR32_PST_KD, &d, 1);
}

void MicroRobot::setSpeed(uint8_t index, int16_t speed) {
  if ((index < 1) || (index > 4))  return;
  int16_t speedBuf;
  if (speed == BRAKE) {
    speedBuf = speed;
  }
  else {
    if ((motorMode[index-1] == MODE_OPEN) | (motorMode[index-1] == MODE_DIR_OPEN)) {
		switch(Multiple[index - 1]){
        case 0x40: speed=constrain(speed,-255,255);break;
        case 0x20: speed=constrain(speed,-511,511);break;
        case 0x10: speed=constrain(speed,-1023,1023);break;
        case 0x8: speed=constrain(speed,-2047,2047);break;
        case 0x4: speed=constrain(speed,-4095,4095);break;
        case 0x2: speed=constrain(speed,-8191,8191);break;
        case 0x1: speed=constrain(speed,-16383,16383);break;
		default:break;	
      }       
      speedBuf = speed * Multiple[index-1];
    }
    //else if ((motorMode[index-1] == MODE_SPEED) | (motorMode[index-1] == MODE_DIR_SPEED)) {
      else{
      float _speedBuf = ((float)speed / 100.0);
      speedBuf = ratio[index - 1] * resolution[index - 1] * _speedBuf;
    }
  }
  write16(index, ADDR16_SET_SPEED, &speedBuf, 1);
}

void MicroRobot::setPosition(uint8_t index, int16_t position) {
  if ((index < 1) || (index > 4))  return;
  if ((motorMode[index-1] == MODE_POSITION) | (motorMode[index-1] == MODE_DIR_POSITION)) {
    write16(index, ADDR16_SET_POSITION, &position, 1);
  }
}

int16_t MicroRobot::getRatio(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return ratio[index - 1];
}

int8_t MicroRobot::getResolution(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return resolution[index - 1];
}

int8_t MicroRobot::getMode(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get8_t(index, ADDR8_MODE);
}

bool MicroRobot::getFault(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return (bool)get8_t(index, ADDR8_FAULT);
}

float MicroRobot::getS_PID_P(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get32_t(index, ADDR32_SPD_KP);
}

float MicroRobot::getS_PID_I(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get32_t(index, ADDR32_SPD_KI);
}

float MicroRobot::getS_PID_D(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get32_t(index, ADDR32_SPD_KD);
}

float MicroRobot::getP_PID_P(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get32_t(index, ADDR32_PST_KP);
}

float MicroRobot::getP_PID_I(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get32_t(index, ADDR32_PST_KI);
}

float MicroRobot::getP_PID_D(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get32_t(index, ADDR32_PST_KD);
}

int16_t MicroRobot::getSetSpeed(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  int16_t speedBuf=get16_t(index, ADDR16_SET_SPEED);
  if ((speedBuf<16384)||(speedBuf>-16384)) {
    if ((motorMode[index-1] == MODE_OPEN) | (motorMode[index-1] == MODE_DIR_OPEN)) {
      speedBuf=speedBuf/Multiple[index-1];
    }
    else if ((motorMode[index-1] == MODE_SPEED) | (motorMode[index-1] == MODE_DIR_SPEED)) {
	  float getSpeedBuf;
      getSpeedBuf = (float)speedBuf/(float)(ratio[index - 1] * resolution[index - 1]);
	  speedBuf = getSpeedBuf*100;
    }
  }
  return speedBuf;
}

int16_t MicroRobot::getSetPosition(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get16_t(index, ADDR16_SET_POSITION);
}

int16_t MicroRobot::getSpeedRaw(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get16_t(index, ADDR16_REAL_SPEED);
}

int16_t MicroRobot::getSpeed(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  int32_t realSpeed = get16_t(index, ADDR16_REAL_SPEED);
  realSpeed = (realSpeed * 100) / (ratio[index - 1] * resolution[index - 1]);
  return (int16_t)realSpeed;
}

int16_t MicroRobot::getPosition(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get16_t(index, ADDR16_REAL_POSITION);
}

uint8_t MicroRobot::getMotorVersion(uint8_t index) {
  if ((index < 1) || (index > 4))  return 0;
  return get8_t(index, ADDR8_VERSION);
}
//********************************************************

//**************************private：motor******************************
void MicroRobot::write8(uint8_t index, uint8_t addr, uint8_t *data, uint8_t len) {
  Wire.beginTransmission(MotorAddress[index - 1]); // transmit to device
  Wire.write(addr);              // sends one byte
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data[i]);              // sends one byte
  }
  Wire.endTransmission();    // stop transmitting
}

void MicroRobot::write16(uint8_t index, uint8_t addr, int16_t *data, uint8_t len) {
  Wire.beginTransmission(MotorAddress[index - 1]); // transmit to device
  Wire.write(addr * 2);            // sends one byte
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data[i] >> 8);            // sends one byte
    Wire.write(data[i] & 0xFF);            // sends one byte
  }
  Wire.endTransmission();    // stop transmitting
}

void MicroRobot::write32(uint8_t index, uint8_t addr, float *data, uint8_t len) {
  Wire.beginTransmission(MotorAddress[index - 1]); // transmit to device
  Wire.write(addr * 4);            // sends one byte
  for (uint8_t i = 0; i < len; i++) {
    val1.pid = data[i];
    Wire.write(val1.Pid[3]);              // sends one byte
    Wire.write(val1.Pid[2]);              // sends one byte
    Wire.write(val1.Pid[1]);              // sends one byte
    Wire.write(val1.Pid[0]);              // sends one byte
  }
  Wire.endTransmission();    // stop transmitting
}

int8_t MicroRobot::get8_t(uint8_t index, uint8_t ADDR) {
  int8_t onebyte;
  Wire.beginTransmission(MotorAddress[index - 1]); // transmit to device
  Wire.write(ADDR);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  Wire.requestFrom(MotorAddress[index - 1], (uint8_t)1);
  char i = 0;
  while (Wire.available())  {
    if (i < 0) {
      return 0;
    }
    onebyte = Wire.read(); // receive a byte as character
    i--;
  }
  return onebyte;
}

int16_t MicroRobot::get16_t(uint8_t index, uint8_t ADDR) {
  Wire.beginTransmission(MotorAddress[index - 1]); // transmit to device
  Wire.write(ADDR * 2);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  Wire.requestFrom(MotorAddress[index - 1], (uint8_t)2);
  char i = 1;
  while (Wire.available())  {
    if (i < 0) {
      return 0;
    }
    val2.Twobyte[i] = Wire.read(); // receive a byte as character
    i--;
  }
  return val2.twobyte;
}

float MicroRobot::get32_t(uint8_t index, uint8_t ADDR) {
  Wire.beginTransmission(MotorAddress[index - 1]); // transmit to device
  Wire.write(ADDR * 4);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  Wire.requestFrom(MotorAddress[index - 1], (uint8_t)4);
  char i = 3;
  while (Wire.available())  {
    if (i < 0) {
      return 0;
    }
    val3.Fourbyte[i] = Wire.read(); // receive a byte as character
    i--;
  }
  return val3.fourbyte;
}

//********************************************************

