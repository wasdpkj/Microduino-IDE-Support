#include <ironman_shield_coreNrf.h>

ironmanShieldNrf shieldNrf;

int song[] = {
  262, 262, 294, 262, 349, 330,
  262, 262, 294, 262, 392, 349,
  262, 262, 523, 440, 349, 330, 294,
  494, 494, 440, 349, 392, 349
};

int noteDurations[] = {
  4, 4, 2, 2, 2, 1,
  4, 4, 2, 2, 2, 1,
  4, 4, 2, 2, 2, 2, 2,
  4, 4, 2, 2, 2, 1
};

void song_play()
{
  for (int thisNote = 0; thisNote < 25; thisNote++)
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    shieldNrf.tone(song[thisNote], noteDuration);
    delay(noteDuration);
    delay(50);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(shieldNrf.begin());
  Serial.println("Start");
  pinMode(6, INPUT_PULLUP);
}

void loop() {
  if (!digitalRead(6)) {
    shieldNrf.priorityEN(AVRMODE);
    Serial.println("playing........");
    song_play();
  }
}