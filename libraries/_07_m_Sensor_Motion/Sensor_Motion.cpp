// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Machine  maxing@microduino.cc
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// Microduino IDE Support：
// https://github.com/wasdpkj/Microduino-IDE-Support/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

// ==============
// E-mail:
// Machine
// maxing@microduino.cc

#include "Sensor_Motion.h"

#if defined (ESP32)
sensorMotion::sensorMotion(HardwareSerial *ser, int _rx, int _tx) {
  motionHwSerial = ser;
  pinRX = _rx;
  pinTX = _tx;
}
#elif defined (__AVR__)
sensorMotion::sensorMotion(SoftwareSerial *ser) {
  motionSwSerial = ser;
}

sensorMotion::sensorMotion(HardwareSerial *ser) {
  motionHwSerial = ser;
}
#endif

void sensorMotion::begin() {
#if defined (ESP32)
  motionHwSerial->begin(9600, SERIAL_8N1, pinRX, pinTX); //上位机设为9600波特率（假设下位机曾经是57600波特率，但下位机重启后恢复了9600默认波特率）
  delay(50);
  motionHwSerial->write("at+3:3\r\n");//向下位机发送设定57600波特率的at指令
  delay(50);
  motionHwSerial->begin(57600, SERIAL_8N1, pinRX, pinTX); //上位机设为57600波特率（下位机当前波特率）
  delay(50);
#elif defined (__AVR__)
  if (motionHwSerial) {//如果是硬串口，使用57600波特率与sensor_motion通信
    motionHwSerial->begin(9600);//上位机设为9600波特率（假设下位机曾经是57600波特率，但下位机重启后恢复了9600默认波特率）
    delay(50);
    motionHwSerial->write("at+3:3\r\n");//向下位机发送设定57600波特率的at指令
    delay(50);
    motionHwSerial->begin(57600);//上位机设为57600波特率（下位机当前波特率）
    delay(50);
  }
  else if (motionSwSerial) {//如果是软串口，使用9600波特率与sensor_motion通信
    motionSwSerial->begin(9600);//上位机设为9600波特率（下位机上电默认9600波特率）
  }
#endif
  //  else if (motionSwSerial) {//如果是硬串口，使用9600波特率与sensor_motion通信
  //    motionSwSerial->begin(57600);//上位机设为57600波特率（假设下位机已经是57600波特率，而上位机重启了，下位机没重启）
  //    delay(50);
  //    motionSwSerial->write("at+3:0\r\n");//向下位机发送设定9600波特率的at指令
  //    delay(50);
  //    motionSwSerial->begin(9600);//上位机设为9600波特率（下位机当前波特率）
  //    delay(50);
  //  }
}
uint8_t rfStep = STEP_WAIT_AA;
uint8_t rfIndex = 0;
uint8_t checkSum = 0;
uint8_t  rfBuf[20];
uint8_t rfLen = 0;
int the_count = 0, the_error = 0;

bool sensorMotion::parseData(uint8_t _cmd) {
  uint8_t temp;
  //从串口读取1字节
  if (motionHwSerial) {
    temp = motionHwSerial->read();
  }
#if defined (__AVR__)
  else if (motionSwSerial) {
    temp = motionSwSerial->read();
  }
#endif
  /*
    if (temp != 0xff) {
    Serial.print(temp, HEX); Serial.print(' ');
    }
  */
  delayMicroseconds(200);//等待200微妙，避免读取速度太快，串口缓存读取错误
  //判断数据头：0x7a
  if (rfStep == STEP_WAIT_AA && temp == STX_AA) {
    rfStep = STEP_WAIT_BB;
    //Serial.print(temp, HEX); Serial.print(' ');
  }
  //判断数据头：0x7b
  else if (rfStep == STEP_WAIT_BB) {
    if (temp == STX_BB) {
      rfStep = STEP_WAIT_CMD;
      //Serial.print(temp, HEX); Serial.print(' ');
    }
    else {
      rfStep = STEP_WAIT_AA;
    }
  }
  //判断数据类型与指定接收类型是否一致，并根据指定接收类型设置数据接收长度
  else if (rfStep == STEP_WAIT_CMD) {
    if (temp == _cmd) {
      //Serial.print(temp, HEX); Serial.print(' ');
      rfStep = STEP_WAIT_DATA;
      rfIndex = 0;
      checkSum = 0;
      switch (_cmd) {
        case 1: rfLen = 6;  break;
        case 2: rfLen = 9;  break;
        case 3: rfLen = 12; break;
        case 4: rfLen = 18; break;
      }
    }
    else {
      rfStep = STEP_WAIT_AA;
    }
  }
  //按照数据接收长度接收数据，并且进行加法校验
  else if (rfStep == STEP_WAIT_DATA) {
    rfBuf[rfIndex] = temp;
    //Serial.print(temp, HEX); Serial.print(' ');
    rfIndex++;
    checkSum += temp;

    if (rfIndex == rfLen) {
      rfStep = STEP_WAIT_SUM;
    }
  }
  //判断校验位与加法校验值（为满足firmata格式，加法校验均按位与上0x7f）是否一致
  else if (rfStep == STEP_WAIT_SUM) {
    rfStep = STEP_WAIT_AA;
    //the_count++;//记录接收次数
    //Serial.print(temp, HEX); Serial.print(' ');
    if (temp == (checkSum & 0x7f)) {
      //Serial.println("check");
      return true;
    }
    else {
      //Serial.println("error");
      //the_error++;//记录误码次数
    }
  }
  return false;
}

//清串口缓存
void sensorMotion::mySerialFlush() {
  if (motionHwSerial) {
    while (motionHwSerial->read() >= 0);
  }
#if defined (__AVR__)
  else if (motionSwSerial) {
    while (motionSwSerial->read() >= 0);
  }
#endif
}

//将两个byte移位组合起来
int16_t combine_a_number(byte h, byte l) {
  int16_t result;
  ((int8_t *)&result)[0] = rfBuf[l]; // LSB
  ((int8_t *)&result)[1] = rfBuf[h]; // MSB
  return result;
}

uint32_t last_time;
boolean sensorMotion::getData(uint8_t _cmd, float *_array) {
  last_time = millis();//开始计时
  mySerialFlush();//清串口缓存

  while (!parseData(_cmd)) {
    //超时300ms收不到正确数据
    if (millis() - last_time > 1500) {
      Serial.println("time_out");
      begin();//重新设置波特率
      if (_cmd == MOTION_3) {
        if (motionHwSerial) {
          motionHwSerial->write("at+0:1\r\n");//让下位机发送3轴姿态角
          delay(50);
          motionHwSerial->write("at+6:0\r\n");//让下位机发送2字节格式数据
          delay(50);
        }
#if defined (__AVR__)
        else if (motionSwSerial) {
          motionSwSerial->write("at+0:1\r\n");//让下位机发送3轴姿态角
          delay(50);
          motionSwSerial->write("at+6:0\r\n");//让下位机发送2字节格式数据
          delay(50);
        }
#endif
      }
      else if (_cmd == RAW_6) {
        if (motionHwSerial) {
          motionHwSerial->write("at+0:0\r\n");//让下位机发送6轴原始值
          delay(50);
          motionHwSerial->write("at+6:0\r\n");//让下位机发送2字节格式数据
          delay(50);
        }
#if defined (__AVR__)
        else if (motionSwSerial) {
          motionSwSerial->write("at+0:0\r\n");//让下位机发送6轴原始值
          delay(50);
          motionSwSerial->write("at+6:0\r\n");//让下位机发送2字节格式数据
          delay(50);
        }
#endif
      }
      return false;
    }
  }

  //成功接收到3轴姿态角，写入数组
  if (_cmd == MOTION_3) {
    _array[0] = combine_a_number(0, 1) / 100.0f;
    _array[1] = combine_a_number(2, 3) / 100.0f;
    _array[2] = combine_a_number(4, 5) / 100.0f;
  }
  //成功接收到6轴原始值，写入数组
  else if (_cmd = RAW_6) {
    _array[0] = combine_a_number(0, 1);
    _array[1] = combine_a_number(2, 3);
    _array[2] = combine_a_number(4, 5);
    _array[3] = combine_a_number(6, 7);
    _array[4] = combine_a_number(8, 9);
    _array[5] = combine_a_number(10, 11);
  }

  //记录100次接收中的误码率
  //  if (the_count >= 100) {
  //    Serial.print(the_error);
  //    Serial.print(" error in ");
  //    Serial.print(the_count);
  //    Serial.print(" times");
  //    while (1);
  //  }
  return true;
}

//设定发送间隔，输入值1~9，代表下位机算几次发送1次
void sensorMotion::set_transmit_interval(byte a) {
  if (a > 0 && a < 10) {
    if (motionHwSerial) {
      motionHwSerial->print("at+1:");
      motionHwSerial->print(int2char(a));
      motionHwSerial->print("\r\n");
    }
#if defined (__AVR__)
    else if (motionSwSerial) {
      motionSwSerial->print("at+1:");
      motionSwSerial->print(int2char(a));
      motionSwSerial->print("\r\n");
    }
#endif
    delay(150);
  }
}
//设定加速度计滤波系数，输入值1~9，代表旧值的权重0.1~0.9
void sensorMotion::set_smoothness(byte a) {
  if (a > 0 && a < 10) {
    if (motionHwSerial) {
      motionHwSerial->print("at+2:");
      motionHwSerial->print(int2char(a));
      motionHwSerial->print("\r\n");
    }
#if defined (__AVR__)
    else if (motionSwSerial) {
      motionSwSerial->print("at+2:");
      motionSwSerial->print(int2char(a));
      motionSwSerial->print("\r\n");
    }
#endif
    delay(150);
  }
}
//设定加速度计分辨率，输入值0~3，代表满量程2、4、8、16g
void sensorMotion::setFullScaleAccelRange(uint8_t _range) {
  if (motionHwSerial) {
    motionHwSerial->print("at+4:");
    motionHwSerial->print(int2char(_range));
    motionHwSerial->print("\r\n");
  }
#if defined (__AVR__)
  else if (motionSwSerial) {
    motionSwSerial->print("at+4:");
    motionSwSerial->print(int2char(_range));
    motionSwSerial->print("\r\n");
  }
#endif
  delay(150);
}
//设定角速度计分辨率，输入值0~3，代表满量程250、500、1000、2000°/s
void sensorMotion::setFullScaleGyroRange(uint8_t _range) {
  if (motionHwSerial) {
    motionHwSerial->print("at+5:");
    motionHwSerial->print(int2char(_range));
    motionHwSerial->print("\r\n");
  }
#if defined (__AVR__)
  else if (motionSwSerial) {
    motionSwSerial->print("at+5:");
    motionSwSerial->print(int2char(_range));
    motionSwSerial->print("\r\n");
  }
#endif
  delay(150);
}

//数字转ascii码
char sensorMotion::int2char(int b) {
  char tmp = 'e';
  if (b >= 0 && b <= 10) {
    tmp = (char)(b + 48);
  }
  return tmp;
}
