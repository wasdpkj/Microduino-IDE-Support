/**************************************************************
  Microduino_GSM_GPRS模块维基网址:
  https://wiki.microduino.cn/index.php/Microduino-Module_GPRS/zh
  
  本示例给出了使用GSM模块的一些方法：
  1、获取设备的信号强度（SignalQuality）
  2、获取设备的身份辨识码（IMEI）
  3、发送短信
 **************************************************************/

#include <TinyGsmClient.h>

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define SerialAT Serial1
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); /* RX:D2, TX:D3 */
#endif

/**
**CoreESP32 UART: [HardwareSerial]
**/
#if defined (ESP32)
HardwareSerial SerialAT(1);
#endif

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

const char number[] = "+86xxxxxxxxxxx";
const char text[] = "Hello Microduino!";

void setup() {
  // Set console baud rate
  Serial.begin(9600);
  delay(10);

  // Set GSM module baud rate
#if defined (__AVR__)
  SerialAT.begin(9600);
#elif defined (ESP32)
  SerialAT.begin(9600, SERIAL_8N1, D2, D3); /* RX:D2, TX:D3 */
#endif
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();

  //Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  Serial.println("Search Network");
  if (!modem.waitForNetwork()) {
    Serial.println("Network Error!");
    while (1);
  }
  Serial.print("SignalQuality:");
  Serial.println(modem.getSignalQuality());
  Serial.print("getIMEI:");
  Serial.println(modem.getIMEI());

  modem.sendSMS(number, text);
}

void loop() {

}
