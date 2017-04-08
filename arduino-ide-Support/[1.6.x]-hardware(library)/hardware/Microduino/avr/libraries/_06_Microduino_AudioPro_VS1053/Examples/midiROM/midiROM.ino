
#include <AudioPro.h>
#include "file.h"

SdFat sd;
AudioPro midiPlayer;

void setup() {
 
  uint8_t result; //result code from some function as to be tested at later time.
  pinMode(MP3_XCS,OUTPUT);
  digitalWrite(MP3_XCS, HIGH);
  Serial.begin(115200);

  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  Serial.print(FreeRam(), DEC);  // FreeRam() is provided by SdFatUtil.h
  Serial.println(F(" Should be a base line of 1017, on ATmega328 when using INTx"));

  sd.begin(SD_SEL, SPI_FULL_SPEED);

  //Initialize the MP3 Player Shield
  result = midiPlayer.begin();
  //check result, see readme for error codes.
  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
    if( result == 6 ) {
      Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
      Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
    }
  }
  midiPlayer.setVolume(10, 10);
  Serial.println(F("pleast input 'a' or 'm' to play the midi file:."));
}

void loop() {
  if(Serial.available()){
     char c = Serial.read();
     if(c == 'a')
        midiPlayer.playMIDInote(Array, sizeof(Array));  
     else if(c == 'm')
        midiPlayer.playMIDInote(marselje, sizeof(marselje)); 
  }
}
