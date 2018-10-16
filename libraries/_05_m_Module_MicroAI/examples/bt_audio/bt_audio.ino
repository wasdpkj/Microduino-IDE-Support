#include <Microduino_Key.h>
#include <Microduino_Ai.h>

#define BT_NAME     "my MicroAi"

DigitalKey touch1(D4);
DigitalKey touch2(D6);
MicroAi mAi(&Serial1);

void setup() {
  Serial.begin(115200);
  touch1.begin(INPUT_PULLUP);
  touch2.begin(INPUT_PULLUP);   

  Serial.println("MicroAi set mode: BT.");
  Serial.println("start initiation.");     
  if(mAi.begin(MODE_BT)){
    Serial.println("init OK!");  
  }else{
    Serial.println("init failed!");
    while(1);  
  }

  if(mAi.nameBT(BT_NAME)){
    Serial.println("set bt name OK!");  
  }else{
    Serial.println("set bt name failed!");
    while(1);  
  }   
    
  delay(1000);
}

void loop() {

  if (touch2.readEvent() == SHORT_PRESS) {
    mAi.cmdBT(CMD_FORWARD);     
  }

  if (touch1.readEvent() == SHORT_PRESS) {
    mAi.cmdBT(CMD_PAUSE);  
  }
  
  delay(50);
}





