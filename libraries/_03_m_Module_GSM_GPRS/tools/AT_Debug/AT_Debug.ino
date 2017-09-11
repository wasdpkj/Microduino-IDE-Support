/**************************************************************
  本示例给出了自动检测GSM模块波特率的方法
  并可以直接用AT命令访问
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

// Module baud rate
uint32_t rate = 0; // Set to 0 for Auto-Detect

void setup() {
  // Set console baud rate
  SerialMon.begin(9600);
  delay(5000);
}

void loop() {

  if (!rate) {
    static uint32_t rates[] = { 115200, 9600, 57600, 19200, 38400, 74400, 74880, 230400, 460800, 2400, 4800, 14400, 28800 };

    SerialMon.println("Autodetecting baud rate");
    for (unsigned i = 0; i < sizeof(rates) / sizeof(rates[0]); i++) {
      SerialMon.print(String("Trying baud rate ") + rates[i] + "... ");
#if defined (ESP32)
      SerialAT.begin(rates[i], SERIAL_8N1, D2, D3);
#elif defined (__AVR__)
      SerialAT.begin(rates[i]);
#endif
      delay(10);
      if (modem.autoBaud(1000)) {
        rate = rates[i];
        SerialMon.println(F("OK"));
        break;
      } else {
        SerialMon.println(F("fail"));
      }

    }
  }

  if (!rate) {
    SerialMon.println(F("***********************************************************"));
    SerialMon.println(F(" Module does not respond!"));
    SerialMon.println(F("   Check your Serial wiring"));
    SerialMon.println(F("   Check the module is correctly powered and turned on"));
    SerialMon.println(F("***********************************************************"));
    delay(30000L);
    return;
  }

  SerialAT.begin(rate);

  // Access AT commands from Serial Monitor
  SerialMon.println(F("***********************************************************"));
  SerialMon.println(F(" You can now send AT commands"));
  SerialMon.println(F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
  SerialMon.println(F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
  SerialMon.println(F("***********************************************************"));

  while (true) {
    if (SerialAT.available()) {
      SerialMon.write(SerialAT.read());
    }
    if (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
    delay(0);
  }
}
