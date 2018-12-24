#include <Microduino_Ai.h>

#define WIFI_SSID   "ssid"
#define WIFI_PWD    "password"

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
MicroAi mAi(&mySerial);
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
MicroAi mAi(&Serial1);
#endif

#if defined (ESP32)
HardwareSerial mySerial1(1);  //UART1
MicroAi mAi(&mySerial1, D2, D3);
#endif

void setup() {
  Serial.begin(115200);

  Serial.print("MicroAi set mode: TEST.");
  Serial.println("start initiation. ");     
  if(mAi.begin(MODE_TEST)){
    Serial.println("init OK!");  
  }else{
    Serial.println("init failed!");
    while(1);  
  }
  
  if(mAi.setWifi(WIFI_SSID, WIFI_PWD)){
    Serial.println("set wifi ssid and password OK!");  
  }else{
    Serial.println("set wifi ssid and password failed!");
    while(1);  
  }

  delay(1000);
}

void loop() {

  Serial.print("Test result: ");
  Serial.println(mAi.testQuery(), HEX);
  delay(1000);
}





