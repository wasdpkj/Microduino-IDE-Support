/**************************************************************

   ATTENTION! Downloading big files requires quite a lot
   of knowledge - so this is for more experienced developers.

 **************************************************************/

// Increase RX buffer
#define TINY_GSM_RX_BUFFER 1030

#include <TinyGsmClient.h>
#include <CRC32.h>

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

uint32_t knownCRC32 = 0x6f50d767;
uint32_t knownFileSize = 1024;   // In case server does not send it

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

void printPercent(uint32_t readLength, uint32_t contentLength) {
  // If we know the total length
  if (contentLength != -1) {
    Serial.print("\r ");
    Serial.print((100.0 * readLength) / contentLength);
    Serial.print('%');
  } else {
    Serial.println(readLength);
  }
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

  // if you get a connection, report back via serial:
  if (!client.connect(server, 80)) {
    Serial.println(" fail");
    delay(10000);
    return;
  }
  Serial.println(" OK");
  // Make a HTTP request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000L) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(10000L);
      return;
    }
  }

  Serial.println("Reading response header");
  uint32_t contentLength = knownFileSize;

  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();
    //Serial.println(line);    // Uncomment this to show response header
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.length() == 0) {
      break;
    }
  }

  Serial.println("Reading response data");
  timeout = millis();
  uint32_t readLength = 0;
  CRC32 crc;

  unsigned long timeElapsed = millis();
  printPercent(readLength, contentLength);
  while (readLength < contentLength && client.connected() && millis() - timeout < 10000L) {
    while (client.available()) {
      uint8_t c = client.read();
      //Serial.print((char)c);       // Uncomment this to show data
      crc.update(c);
      readLength++;
      if (readLength % (contentLength / 13) == 0) {
        printPercent(readLength, contentLength);
      }
      timeout = millis();
    }
  }
  printPercent(readLength, contentLength);
  timeElapsed = millis() - timeElapsed;
  Serial.println();

  client.stop();
  Serial.println("Server disconnected");

  modem.gprsDisconnect();
  Serial.println("GPRS disconnected");
  Serial.println();

  float duration = float(timeElapsed) / 1000;

  Serial.print("Content-Length: ");   Serial.println(contentLength);
  Serial.print("Actually read:  ");   Serial.println(readLength);
  Serial.print("Calc. CRC32:    0x"); Serial.println(crc.finalize(), HEX);
  Serial.print("Known CRC32:    0x"); Serial.println(knownCRC32, HEX);
  Serial.print("Duration:       ");   Serial.print(duration); Serial.println("s");

  // Do nothing forevermore
  while (true) {
    delay(1000);
  }
}
