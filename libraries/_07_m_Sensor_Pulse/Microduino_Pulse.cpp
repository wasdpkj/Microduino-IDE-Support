/*********************************************************
//  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

//  版权所有：
//  @小崔  cuiwenjing@microduino.cc

// 支持Microduino_Sensor_Pulse

// Microduino wiki:
// http://wiki.microduino.cn

// E-mail:
// Wenjing Cui
// cuiwenjing@microduino.cc

// Weibo:
// @路蝶-6

//日期：2017.07
*********************************************************/

#include "Microduino_Pulse.h"

Pulse::Pulse(uint8_t _Pin) {
  pulseKey = new DigitalKey(_Pin);
}

void Pulse::begin() {
  pulseKey->begin();
}

/*获取两个脉冲之间的时间差*/
uint32_t Pulse::getInterval() {
  randomSeed(analogRead(A7));
  delay(random(1,5));
  pinState = pulseKey->readVal();
  if (pinState == KEY_RELEASING) {
    lastTime = thisTime;
    thisTime = millis();
    return (thisTime - lastTime);
  }
  else {
    return 0;
  }
}

bool Pulse::isUsefull(uint32_t _t) {
  if (_t) {  //有数据时才执行滤波
    if (_t < TIME1_MAX) {   //滤最小阈值
      if (_t < TIME1_MIN) { //若有过快的波，开启标志位
        time1_sta_error = true;
      }
    }
    else {
      if (!time1_sta_error) {  //如果被标志 则本次波形无效
        return true;
      }
      time1_sta_error = false;
    }
  }
  return false;
}

uint32_t Pulse::pulse(uint32_t _t) {
  if (!_t)
    return 0;

  uint32_t  _pulse_vol = 60000 / _t;

  if ((_pulse_vol > PULSE_MIN) && (_pulse_vol < PULSE_MAX)) {
    pulse_vol += _pulse_vol;
    pulse_num++;
    if (pulse_num > (PULSE_UPDATA - 1)) {
      uint32_t _pulse = pulse_vol / PULSE_UPDATA;
      pulse_num = 0;
      pulse_vol = 0;
      pulse_cache = _pulse;
      return _pulse;
    }
    else {
      return 0;
    }
  }
  else {
    pulse_num = 0;
    pulse_vol = 0;
    return 0;
  }
}

bool Pulse::available() {
  uint32_t pulse1 = getInterval();
  if (isUsefull(pulse1)) {
    pulse_t = pulse(pulse1);
    if (pulse_t) {
      return true;
    }
  }
  return false;
}


uint16_t Pulse::getPulse() {
  return  pulse_t;
}

