#include <ESP8266.h>
#define HOST_NAME   "pool.ntp.org"
#define HOST_PORT   (123)
#define TIME_ZONE +8
/*==============================================================================*/
/* Useful Constants */
#define SECS_PER_HOUR (3600UL)

#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */

#define EspSerial mySerial
#define UARTSPEED  9600
#endif

#define SSID        "Makermodule"
#define PASSWORD    "microduino"

ESP8266 wifi(&EspSerial);

#include <TimeLib.h>

uint8_t buffer[128] = {0};
static uint8_t upd_id = 0;
uint32_t len = 0;
int Year, Month, Day, Hour, Minute, Second, Weekday;
time_t prevDisplay = 0;

void updateTimeData() {
  do {
    delay(200);
    registerUDPAndSendRecvData();
    if (len > 0) {
      getTimeStampAndSetRTC();
      unregisterUDP();
    } else {
      unregisterUDP();
    }
  } while (!len);
}

void getTimeStampAndSetRTC() {
  Serial.print("Received:[");
  unsigned long t = (((unsigned long)buffer[40] << 24) |
                     ((unsigned long)buffer[41] << 16) |
                     ((unsigned long)buffer[42] <<  8) |
                     (unsigned long)buffer[43]) - 2208988800UL;

  Serial.print("Unix timestamp:");
  Serial.print(t);
  Serial.print("]\r\n");

  setTime(t);
  adjustTime(TIME_ZONE * SECS_PER_HOUR);
}

void registerUDPAndSendRecvData() {
  if (wifi.registerUDP(upd_id, HOST_NAME, HOST_PORT)) {
    Serial.print("register udp ");
    Serial.print(upd_id);
    Serial.println(" ok");
  } else {
    Serial.print("register udp ");
    Serial.print(upd_id);
    Serial.println(" err");
  }

  static const char PROGMEM
  timeReqA[] = { 227,  0,  6, 236 }, timeReqB[] = {  49, 78, 49,  52 };
  // Assemble and issue request packet
  uint8_t       buf[48];
  memset(buf, 0, sizeof(buf));
  memcpy_P( buf    , timeReqA, sizeof(timeReqA));
  memcpy_P(&buf[12], timeReqB, sizeof(timeReqB));

  wifi.send(upd_id, (const uint8_t*)buf, 48);
  //uint32_t len = wifi.recv(upd_id, buffer, sizeof(buffer), 10000);
  len = wifi.recv(upd_id, buffer, sizeof(buffer), 10000);
}
void unregisterUDP() {
  if (wifi.unregisterUDP(upd_id)) {
    Serial.print("unregister udp ");
    Serial.print(upd_id);
    Serial.println(" ok");
  } else {
    Serial.print("unregister udp ");
    Serial.print(upd_id);
    Serial.println(" err");
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

void setup(void)
{
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print("setup begin\r\n");
  delay(100);

  WifiInit(EspSerial, UARTSPEED);

  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
    while (1);
  }
  wifi.enableMUX();
  Serial.print("setup end\r\n");
  updateTimeData();
}

void loop(void) {
  if (now() != prevDisplay) {
    prevDisplay = now();
    serialClockDisplay(year(), month(), day(), hour(), minute(), second());
  }
}
