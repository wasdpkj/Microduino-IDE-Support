/*
  -------------------------------------------------------
  Musical notes are stored in toneList[].
  -------------------------------------------------------
*/
#define SONG_SUM 11
#define  PLAY_INTERVAL  220

#define TONE_DO   523
#define TONE_RE   587
#define TONE_MI   659
#define TONE_FA   698
#define TONE_SO   784
#define TONE_LA   880
#define TONE_SI   988

//Musical Notes---LOW    1,   2,   3,   4,   5,   6,   7
//                                                MIDDLE    1,   2,   3,   4,   5,   6,   7
//                                                                                      HIGH     1,    2,    3,    4,    5,    6,    7
//Note number in songs:  0    1    2    3    4    5    6    7    8    9    10   11   12   13    14    15    16    17    18    19    20
uint16_t toneList[] = { 262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988, 1046, 1175, 1318, 1397, 1568, 1760, 1967 };

//Song #1: Rag Toss
uint8_t music_1[] = { 12, 10, 12, 10, 12, 10, 9, 10, 12, 12, 12, 10, 13, 12, 10, 12, 10, 9, 8, 9, 10, 12, 10, 9, 8, 9, 10, 0 }; //27 notes.
uint8_t rhythm_1[] = { 4, 2, 4, 2, 2, 2, 2, 2, 8, 2, 4, 2, 4, 4, 2, 2, 2, 2, 4, 4, 4, 4, 2, 2, 2, 2, 8 }; //Number of beats for each note.
//Song #2: Two Tigers
uint8_t music_2[] = { 8, 9, 10, 8, 8, 9, 10, 8, 10, 11, 12, 10, 11, 12, 0 };   //15
uint8_t rhythm_2[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 8 };
//Song #3: Robot Cat
uint8_t music_3[] = { 5, 8, 8, 10, 13, 10, 12, 12, 13, 12, 10, 11, 10, 9, 6, 9, 9, 11, 14, 14, 13, 12, 11, 11, 10, 6, 7, 8, 9 , 0 }; //29
uint8_t rhythm_3[] = { 2, 1, 2, 1, 2, 1, 4, 2, 1, 2, 1, 2, 1, 4, 2, 1, 2, 1, 2, 1, 2, 1, 4, 2, 1, 2, 4, 2, 8 };
//Song #4: Happy Birthday
uint8_t music_4[] = { 5, 5, 6, 5, 8, 7, 5, 5, 6, 5, 9, 8, 5, 5, 12, 10, 8, 7, 6, 11, 11, 10, 8, 9, 8, 0 };  //26
uint8_t rhythm_4[] = { 2, 2, 4, 4, 4, 8, 2, 2, 4, 4, 4, 8, 2, 2, 4, 4, 4, 4, 4, 2, 2, 4, 4, 4, 8 };
//Song #5: Friends Forever
uint8_t music_5[] = { 12, 13, 12, 13, 12, 13, 12, 12, 15, 14, 13, 12, 13, 12, 12, 12, 10, 10, 12, 12, 10, 9, 11, 10, 9, 8, 9, 8, 0  }; //28
uint8_t rhythm_5[] = { 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4 };
//Song #6: Hulu Doll
uint8_t music_6[] = { 8, 8, 10, 8, 8, 10, 22, 13, 13, 13, 12, 13, 12, 8, 10, 22, 15, 13, 13, 12, 13, 12, 8, 9, 22, 14, 14, 12, 10, 12, 0  }; //30
uint8_t rhythm_6[] = { 4, 4, 8, 2, 4, 4, 4, 4, 4, 2, 2, 4, 2, 4, 4, 4, 2, 2, 2, 2, 8, 2, 4, 4, 4, 4, 2, 2, 4, 16 };
//Song #7: Teamwork!
uint8_t music_7[] = { 6, 8, 9, 10, 12, 10, 8, 9, 6, 22, 8, 9, 10, 12, 12, 13, 9, 10, 22, 10, 12, 13, 12, 13, 15, 14, 13, 12, 13, 10, 8, 9, 10, 12, 8, 6, 8, 9, 10, 13, 12 , 0 }; //41
uint8_t rhythm_7[] = { 2, 2, 2, 4, 2, 2, 2, 4, 8, 2, 2, 2, 2, 4, 2, 4, 4, 8, 4, 2, 2, 8, 4, 2, 2, 1, 1, 2, 2, 4, 2, 2, 4, 2, 4, 4, 2, 2, 2, 2, 18 };
//Song #8: Little Apple
uint8_t music_8[] = { 10, 8, 9, 6, 10, 9, 8, 9, 6, 10, 8, 9, 9, 12, 10, 7, 8, 8, 7, 6, 7, 8, 9, 5, 13, 12, 10, 10, 9, 8, 9, 10, 9, 10, 9, 12, 12, 12, 12, 12, 12, 0  }; //41
uint8_t rhythm_8[] = { 4, 4, 4, 4, 2, 2, 2, 2, 8, 4, 4, 4, 4, 2, 2, 4, 4, 2, 2, 4, 2, 2, 4, 4, 2, 2, 4, 4, 2, 4, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4 };
//Song #9: The Chirping Swallow 
uint8_t music_9[] = { 10, 12, 15, 13, 12, 10, 12, 13, 15, 12, 15, 17, 16, 15, 16, 15, 13, 15, 12 , 0 }; //19
uint8_t rhythm_9[] = { 2, 2, 2, 2, 8, 2, 2, 2, 2, 8, 4, 2, 4, 4, 2, 2, 2, 2, 8 };
//Song #10: Spring Time
uint8_t music_10[] = { 10, 10, 10, 8, 5, 5, 22, 10, 10, 10, 8, 10, 22, 12, 12, 10, 8, 5, 5, 5, 6, 7, 8, 10, 9, 0  }; //25
uint8_t rhythm_10[] = { 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4 };
//Song #11: Happy
uint8_t music_11[] = {8, 9, 10, 8, 8, 9, 10, 8, 10, 11, 12, 10, 11, 12, 0};
uint8_t rhythm_11[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2,  4};

const uint8_t * Pmusic[SONG_SUM] = {
  music_1, music_2, music_3, music_4, music_5, music_6, music_7, music_8, music_9, music_10, music_11
};
const uint8_t * Prhythm[SONG_SUM] = {
  rhythm_1, rhythm_2, rhythm_3, rhythm_4, rhythm_5, rhythm_6, rhythm_7, rhythm_8, rhythm_9, rhythm_10, rhythm_11
};

uint8_t playIndex = 0;
uint32_t playTimer = 0;

uint8_t playSound(uint8_t songNum) {
  if (millis() > playTimer && (*(Pmusic[songNum] + playIndex / 2)))
  {
    if (playIndex % 2 == 0)
    {
      if (*(Pmusic[songNum] + playIndex / 2) != 22)
        tone(PIN_BUZZER, toneList[*(Pmusic[songNum] + playIndex / 2) - 1]);
      else
        noTone(PIN_BUZZER);
      playTimer = millis() + *(Prhythm[songNum] + playIndex / 2) * 75;
    }
    else
    {
      noTone(PIN_BUZZER);
      setAllLed(playIndex % 10); 
      playTimer = millis() + PLAY_INTERVAL / 5;
    }
    playIndex++;
  }
  return *(Pmusic[songNum] + playIndex / 2);
}
