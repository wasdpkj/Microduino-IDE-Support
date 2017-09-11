/**************************************************************
 本示例给出了将设置返回到出厂默认设置的方法
 Return settings to Factory Defaults
 **************************************************************/

#include <TinyGsmClient.h>

#define SerialMon Serial

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
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

void setup() {
  // Set console baud rate
  SerialMon.begin(9600);
  delay(10);

  // Set GSM module baud rate
#if defined (__AVR__)
  SerialAT.begin(9600);
#elif defined (ESP32)
  SerialAT.begin(9600, SERIAL_8N1, D2, D3); /* RX:D2, TX:D3 */
#endif
  delay(3000);

  if (!modem.init()) {
    SerialMon.println(F("***********************************************************"));
    SerialMon.println(F(" Cannot initialize modem!"));
    SerialMon.println(F("   Use File -> Examples -> _03_M_Module_GSM_GPRS -> tools -> AT_Debug"));
    SerialMon.println(F("   to find correct configuration"));
    SerialMon.println(F("***********************************************************"));
    return;
  }

  bool ret = modem.factoryDefault();

  SerialMon.println(F("***********************************************************"));
  SerialMon.print  (F(" Return settings to Factory Defaults: "));
  SerialMon.println((ret) ? "OK" : "FAIL");
  SerialMon.println(F("***********************************************************"));
}

void loop() {

}
