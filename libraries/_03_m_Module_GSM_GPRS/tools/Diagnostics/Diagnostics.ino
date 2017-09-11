/**************************************************************
  本示例给出了用GSM模块访问网页并获取网页数据的方法
 **************************************************************/

// Increase buffer fo see less commands
#define TINY_GSM_RX_BUFFER 256

#include <TinyGsmClient.h>

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "YourAPN";
const char user[] = "";
const char pass[] = "";

// Set serial for debug console (to the Serial Monitor, speed 115200)
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
TinyGsmClient client(modem);

const char server[] = "wxdata.weather.com";
const char resource[] = "/wxdata/weather/local/chxx0008?cc=*&unit=m";

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
}

void loop() {
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  if (!modem.restart()) {
    SerialMon.println(" fail");
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is your modem connected properly?"));
    SerialMon.println(F(" Is your serial speed (baud rate) correct?"));
    SerialMon.println(F(" Is your modem powered on?"));
    SerialMon.println(F(" Do you use a good, stable power source?"));
    SerialMon.println(F(" Try useing File -> Examples -> _03_M_Module_GSM_GPRS -> tools -> AT_Debug to find correct configuration"));
    SerialMon.println(F("************************"));
    delay(10000);
  }

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is your sim card locked?"));
    SerialMon.println(F(" Do you have a good signal?"));
    SerialMon.println(F(" Is antenna attached?"));
    SerialMon.println(F(" Does the SIM card work with your phone?"));
    SerialMon.println(F("************************"));
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  SerialMon.print("Connecting to ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println(" fail");
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is GPRS enabled by network provider?"));
    SerialMon.println(F(" Try checking your card balance."));
    SerialMon.println(F("************************"));
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  SerialMon.print("Connecting to ");
  SerialMon.print(server);
  if (!client.connect(server, 80)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  // Skip all headers
  client.find("\r\n\r\n");

  // Read data
  unsigned long timeout = millis();
  unsigned long bytesReceived = 0;
  while (client.connected() && millis() - timeout < 10000L) {
    while (client.available()) {
      char c = client.read();
      //SerialMon.print(c);
      bytesReceived += 1;
      timeout = millis();
    }
  }

  client.stop();
  SerialMon.println("Server disconnected");

  modem.gprsDisconnect();
  SerialMon.println("GPRS disconnected");

  SerialMon.println();
  SerialMon.println("************************");
  SerialMon.print  (" Received: ");
  SerialMon.print(bytesReceived);
  SerialMon.println("bytes");
  SerialMon.print  (" Test:     ");
  SerialMon.println((bytesReceived >= 1000) ? "PASSED" : "FAIL");
  SerialMon.println("************************");

  // Do nothing forevermore
  while (true) {
    delay(1000);
  }
}
