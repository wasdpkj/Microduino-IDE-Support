#include <Microduino_Key.h>
#include <Microduino_Ai.h>

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

#define WIFI_SSID   "ssid"
#define WIFI_PWD    "password"
#define WEB_NUM     4

char* webRadioUrl[WEB_NUM] = {
  "https://http.qingting.fm/386.mp3",
  "https://http.qingting.fm/387.mp3",
  "https://http.qingting.fm/388.mp3",
  "https://http.qingting.fm/389.mp3",
};

static uint8_t webRadioIndex = 0xFF;
DigitalKey touch1(D4);
DigitalKey touch2(D6);

char strBuf[64] = {0};

void setup() {
  Serial.begin(115200);
  touch1.begin(INPUT_PULLUP);
  touch2.begin(INPUT_PULLUP);  

  Serial.print("MicroAi set mode: WIFI.");
  Serial.println("start initiation. ");     
  if(mAi.begin(MODE_WIFI)){
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
  Serial.print("wifi connecting"); 
  while(mAi.getRssi()==0){
    Serial.print("."); 
    delay(1000);
  }
  Serial.println("\r\nwifi connected.");     

  delay(1000);
}

void loop() {

  if(touch1.readEvent() == SHORT_PRESS) {
    if(webRadioIndex < WEB_NUM-1)
      webRadioIndex++;
    else
      webRadioIndex = 0;
      
    mAi.playFile(webRadioUrl[webRadioIndex]);
  }

  if(touch2.readEvent() == SHORT_PRESS) {
    delay(500);
    Serial.print("AT start Rest: ");
    if(mAi.startRest()){
      Serial.println("OK!");  
      int32_t code = mAi.waitResult(strBuf);
      if(code){
        Serial.print("rest code: ");
        Serial.print(code);
        Serial.print(", result: ");
        Serial.println(strBuf);       
      }else{
        Serial.println("rest none ");
      }
    }else{
      Serial.println("Failed!");
    }
  }
  
  delay(50);
}
