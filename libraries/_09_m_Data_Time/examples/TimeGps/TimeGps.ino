#include <Microduino_GPS.h>
#include <TimeLib.h>


//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
#define GPSSerial mySerial
Microduino_GPS GPS(&GPSSerial);
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define GPSSerial Serial1
Microduino_GPS GPS(&GPSSerial);
#endif

//CoreESP32 UART Port: [D2,D3]
#if defined (ESP32)
HardwareSerial GPSSerial(1);
Microduino_GPS GPS(&GPSSerial,D2,D3);
#endif

const int offset = 8;   // 北京时区
time_t prevDisplay = 0;
int Year, Month, Day, Hour, Minute, Second;
uint32_t timer = millis();
void setup()
{
  Serial.begin(115200);
  GPS.begin(38400);  //9600/19200/38400/57600/115200
  GPS.set_powermode(CONTINUOUS);  //CONTINUOUS/POWERSAVE
  GPS.set_updata(UPDATE_2HZ);  //UPDATE_1HZ/UPDATE_2HZ/UPDATE_4HZ/UPDATE_5HZ
  GPS.set_config(CONFIG_SAVE);  //CONFIG_DEFAULT/CONFIG_SAVE
}


void loop()
{
  //如果本地时间已经过校准（gps校准）
  if (timeStatus() != timeNotSet) {
    //如果本地时间发生变化，则更新串口显示
    if (now() != prevDisplay) { 
      prevDisplay = now();
      Serial.println("Location-time: ");
      serialClockDisplay(year(), month(), day(), hour(), minute(), second());
    }
  }
  //如果本地时间还未校准（gps校准）
  else {
    //读取gps信号
    char c = GPS.read();
    if (GPS.newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
      //Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
      if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        return; // we can fail to parse a sentence in which case we should just wait for another
    }
    if (timer > millis()) timer = millis();
    
    //每秒输出一次gps时间
    if (millis() - timer > 1000) {
      timer = millis(); // reset the timer
      Year = GPS.year;
      Month = GPS.month;
      Day = GPS.day;
      Hour = GPS.hour;
      Minute = GPS.minute;
      Second = GPS.seconds;
      Serial.println("GPS-time: ");
      serialClockDisplay(Year, Month, Day, Hour, Minute, Second);
      
      //如果gps时间中的年月日数据都已出现，则开始校准本地时间
      if (Year != 0 && Month != 0 && Day != 0) {
        Serial.println("Calibration complete!");
        Serial.println();
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(offset * SECS_PER_HOUR);//时区转换
      }
    }
  }
}

//*****串口打印日期时间*****
void serialClockDisplay(int _year, int _month, int _day, int _hour, int _minute, int _second) {
  if (_year < 1000) {
    Serial.print("20");
  }
  Serial.print(_year, DEC);
  Serial.print('/');
  if (_month < 10) {
    Serial.print("0");
  }
  Serial.print(_month, DEC);
  Serial.print('/');
  if (_day < 10) {
    Serial.print("0");
  }
  Serial.print(_day, DEC);
  Serial.print("   ");
  Serial.print(_hour, DEC);
  Serial.print(':');
  if (_minute < 10) {
    Serial.print("0");
  }
  Serial.print(_minute, DEC);
  Serial.print(':');
  if (_second < 10) {
    Serial.print("0");
  }
  Serial.println(_second, DEC);
  Serial.println();
}