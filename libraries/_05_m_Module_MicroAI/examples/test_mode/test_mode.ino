#include <Microduino_Ai.h>

#define WIFI_SSID   "ssid"
#define WIFI_PWD    "password"

MicroAi mAi(&Serial1);

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





