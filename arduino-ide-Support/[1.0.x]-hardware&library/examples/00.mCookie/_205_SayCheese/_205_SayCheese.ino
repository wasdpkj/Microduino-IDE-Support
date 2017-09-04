#include <IRremote.h>
#define PHOTO 0xB4B8F

IRsend irsend;

#define mic_pin A0
#define IR_pin 6

#define voice 400

int voice_data;

int num = 10;

long time, timer;

boolean take;

void setup() {
  Serial.begin(115200);
  pinMode(mic_pin, INPUT);
  pinMode(IR_pin, OUTPUT);
}

void loop() {
  voice_data = analogRead(mic_pin);
  //Serial.println(voice_data);
  if (voice_data > voice && take == false)
  {
    time = millis();
    num++;
    delay(10);
    Serial.println(num);
    if (num > 20)
    {
      take = true;
      takePhoto();
      Serial.println("takePhoto...");
    }
  }
  else if (millis() - time > 1000 )//1s之后才可以再次拍摄
  {
    voice_data = analogRead(mic_pin);
    if (voice_data < voice)
    {
      num = 0;
      take = false;
    }
    time = millis();
  }
}

void takePhoto()
{
  for (int i = 0; i < 3; i++)
  {
    irsend.sendSony(PHOTO, 20); // Sony code
    delay(10);
  }
}
