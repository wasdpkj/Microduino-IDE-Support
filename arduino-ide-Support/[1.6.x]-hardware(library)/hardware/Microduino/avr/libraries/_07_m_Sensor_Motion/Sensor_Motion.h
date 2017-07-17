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

#include <SoftwareSerial.h>
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define ACCEL_FS_2          0
#define ACCEL_FS_4          1
#define ACCEL_FS_8          2
#define ACCEL_FS_16         3
#define GYRO_FS_250         0
#define GYRO_FS_500         1
#define GYRO_FS_1000        2
#define GYRO_FS_2000        3

#define  STEP_WAIT_AA     0
#define  STEP_WAIT_BB     1
#define  STEP_WAIT_CMD    2
#define  STEP_WAIT_DATA   3
#define  STEP_WAIT_SUM    4

#define  STX_AA       0x7a
#define  STX_BB       0x7b
#define  MOTION_3   0x01
#define  CMD_3ypr   0x02
#define  RAW_6   0x03
#define  CMD_3accgyro   0x04


class sensorMotion {
  public:
    sensorMotion(SoftwareSerial *ser); // Constructor when using SoftwareSerial
    sensorMotion(HardwareSerial *ser); // Constructor when using HardwareSerial
    void begin(void);
    boolean getData(uint8_t _cmd,float *_array) ;
    void setFullScaleGyroRange(uint8_t _range);
    void setFullScaleAccelRange(uint8_t _range);
    
  private:
    SoftwareSerial *motionSwSerial;
    HardwareSerial *motionHwSerial;
    void buf_process(void);
    void set_transmit_interval(byte a);
    void set_baud_rate(byte a);
    void set_smoothness(byte a);
    char int2char(int b);
    bool parseData(uint8_t _cmd);
    void mySerialFlush(void);
};

