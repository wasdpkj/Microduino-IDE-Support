/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   本例程性能要求高，目前只有Core+ 1284和CoreRF跑得动，需配合Microduino_SD模块使用
*/
#include <Microduino_AudioPro.h>
#include <SD.h>

#define REC_BUTTON 6

AudioPro_FilePlayer musicPlayer = AudioPro_FilePlayer(SD);

File recording;  // the file we will save our recording to
#define RECBUFFSIZE 128  // 64 or 128 bytes.
uint8_t recording_buffer[RECBUFFSIZE];

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit VS1053 Ogg Recording Test");
  pinMode(SD_PIN_SEL, OUTPUT);    //先初始化AudioPro，所以先使能SD卡
  digitalWrite(SD_PIN_SEL, HIGH);
  delay(500);

  // initialise the music player
  if (!musicPlayer.begin()) {
    Serial.println("VS1053 not found");
    while (1);  // don't do anything more
  }

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working

  if (!SD.begin(SD_PIN_SEL)) {
    Serial.println("SD failed, or not present");
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");

  musicPlayer.setVolume(96);  //left & right 0-127
  //or
  //musicPlayer.setVolume(96, 96);  //left right 0-127

  // If DREQ is on an interrupt pin, we can do background
  musicPlayer.useInterrupt(VS1053_PIN_DREQ);  // DREQ int

  // when the button is pressed, record!
  pinMode(REC_BUTTON, INPUT);
  digitalWrite(REC_BUTTON, HIGH);

  // load plugin from SD card! We'll use mono 44.1KHz, high quality
  musicPlayer.applyPatch(recPlugin, sizeof(recPlugin) / sizeof(uint16_t));
}

uint8_t isRecording = false;

void loop() {
  if (!isRecording && !digitalRead(REC_BUTTON)) {
    Serial.println("Begin recording");
    isRecording = true;

    // Check if the file exists already
    char filename[15];
    strcpy(filename, "RECORD00.OGG");
    for (uint8_t i = 0; i < 100; i++) {
      filename[6] = '0' + i / 10;
      filename[7] = '0' + i % 10;
      // create if does not exist, do not open existing, write, sync after write
      if (! SD.exists(filename)) {
        break;
      }
    }
    Serial.print("Recording to "); Serial.println(filename);
    recording = SD.open(filename, FILE_WRITE);
    if (! recording) {
      Serial.println("Couldn't open file to record!");
      while (1);
    }
    musicPlayer.startRecordOgg(true); // use microphone (for linein, pass in 'false')
  }
  if (isRecording)
    saveRecordedData(isRecording);
  if (isRecording && !digitalRead(REC_BUTTON)) {
    Serial.println("End recording");
    musicPlayer.stopRecordOgg();
    isRecording = false;
    // flush all the data!
    saveRecordedData(isRecording);
    // close it up
    recording.close();
    delay(1000);
  }
}

uint16_t saveRecordedData(boolean isrecord) {
  uint16_t written = 0;

  // read how many words are waiting for us
  uint16_t wordswaiting = musicPlayer.recordedWordsWaiting();

  // try to process 256 words (512 bytes) at a time, for best speed
  while (wordswaiting > 256) {
    //Serial.print("Waiting: "); Serial.println(wordswaiting);
    // for example 128 bytes x 4 loops = 512 bytes
    for (int x = 0; x < 512 / RECBUFFSIZE; x++) {
      // fill the buffer!
      for (uint16_t addr = 0; addr < RECBUFFSIZE; addr += 2) {
        uint16_t t = musicPlayer.recordedReadWord();
        //Serial.println(t, HEX);
        recording_buffer[addr] = t >> 8;
        recording_buffer[addr + 1] = t;
      }
      if (! recording.write(recording_buffer, RECBUFFSIZE)) {
        Serial.print("Couldn't write "); Serial.println(RECBUFFSIZE);
        while (1);
      }
    }
    // flush 512 bytes at a time
    recording.flush();
    written += 256;
    wordswaiting -= 256;
  }

  wordswaiting = musicPlayer.recordedWordsWaiting();
  if (!isrecord) {
    Serial.print(wordswaiting); Serial.println(" remaining");
    // wrapping up the recording!
    uint16_t addr = 0;
    for (int x = 0; x < wordswaiting - 1; x++) {
      // fill the buffer!
      uint16_t t = musicPlayer.recordedReadWord();
      recording_buffer[addr] = t >> 8;
      recording_buffer[addr + 1] = t;
      if (addr > RECBUFFSIZE) {
        if (! recording.write(recording_buffer, RECBUFFSIZE)) {
          Serial.println("Couldn't write!");
          while (1);
        }
        recording.flush();
        addr = 0;
      }
    }
    if (addr != 0) {
      if (!recording.write(recording_buffer, addr)) {
        Serial.println("Couldn't write!"); while (1);
      }
      written += addr;
    }
    musicPlayer.sciRead(VS1053_SCI_AICTRL3);
    if (! (musicPlayer.sciRead(VS1053_SCI_AICTRL3) & _BV(2))) {
      recording.write(musicPlayer.recordedReadWord() & 0xFF);
      written++;
    }
    recording.flush();
  }

  return written;
}