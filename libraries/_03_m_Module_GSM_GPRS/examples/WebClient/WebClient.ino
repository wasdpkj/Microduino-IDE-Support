/**************************************************************
 本示例给出了用GSM模块连接网页并获取网页数据的方法
 **************************************************************/

#include <TinyGsmClient.h>

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "YourAPN";
const char user[] = "";
const char pass[] = "";

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

const char server[] = "wxdata.weather.com";
const char resource[] = "/wxdata/weather/local/chxx0008?cc=*&unit=m";

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

  Serial.println("Search Network");
  if (!modem.waitForNetwork()) {
    Serial.println("Network Error!");
    while (1);
  }
  Serial.print("SignalQuality:");
  Serial.println(modem.getSignalQuality());
  Serial.print("getIMEI:");
  Serial.println(modem.getIMEI());

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
}

void loop() {
  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    delay(10000);
    return;
  }
  Serial.println(" OK");

  Serial.print("Connecting to ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    delay(10000);
    return;
  }
  Serial.println(" OK");

  Serial.print("Connecting to ");
  Serial.print(server);
  if (!client.connect(server, 80)) {
    Serial.println(" fail");
    delay(10000);
    return;
  }
  Serial.println(" OK");

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      timeout = millis();
    }
  }
  Serial.println();

  client.stop();
  Serial.println("Server disconnected");

  modem.gprsDisconnect();
  Serial.println("GPRS disconnected");

  // Do nothing forevermore
  while (true) {
    delay(1000);
  }
}
