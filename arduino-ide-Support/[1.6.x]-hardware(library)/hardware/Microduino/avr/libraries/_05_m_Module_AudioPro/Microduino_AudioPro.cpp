// 源自Adafruit_VS1053库
// 修订：@老潘orz  wasdpkj@hotmail.com
// 添加了多种功能和方法
// 播放速度、音量查询、音量整体设置、立体声模式、差分输出、ROM播放、SD音乐文件列表等

/***************************************************
  This is a library for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Microduino_AudioPro.h"

static AudioPro *myself;
static AudioPro_FilePlayer *myself_sd;

#ifndef _BV
#define _BV(x) (1<<(x))
#endif

/*
SIGNAL(TIMER0_COMPA_vect) {
   myself->feedBuffer();
 }
*/
static void feeder(void) {
  myself->feedBuffer();
}

static void feeder_sd(void) {
  myself_sd->feedBuffer();
}

boolean feedBufferLock = false;

#define VS1053_CONTROL_SPI_SETTING  SPISettings(250000,  MSBFIRST, SPI_MODE0)
#define VS1053_DATA_SPI_SETTING     SPISettings(8000000, MSBFIRST, SPI_MODE0)


AudioPro_FilePlayer::AudioPro_FilePlayer(SDClass& _sd, uint8_t midi, uint8_t cs, uint8_t dcs, uint8_t dreq)
  : AudioPro(midi, cs, dcs, dreq), sd(_sd) {

  playingMusic = false;
}

boolean AudioPro_FilePlayer::begin(void) {
  uint8_t v  = AudioPro::begin();
  
  if (!useInterrupt(_dreq)) {
    return 0;
  }
  //Serial.print("Version = "); Serial.println(v);
  return (v == 4);
}

void AudioPro_FilePlayer::end(void) {
  AudioPro::end();

  detachInterrupt(_dreq);
}

boolean AudioPro_FilePlayer::playFullFile(const char *trackname) {
  if (! playMP3(trackname)) return false;

  while (playingMusic) {
    // twiddle thumbs
    feedBuffer();
    delay(5); // give IRQs a chance
  }
  // music file finished!
  return true;
}

void AudioPro_FilePlayer::stopPlaying(void) {
  AudioPro::stopPlaying();
  // wrap it up!
  playingMusic = false;
  currentTrack.close();
}

void AudioPro_FilePlayer::pausePlaying(boolean pause) {
  if (pause) {
    //setAmplifier(false);
    playingMusic = false;
  }
  else {
    //setAmplifier(true);
    playingMusic = true;
    this->feedBuffer();
  }
}

boolean AudioPro_FilePlayer::paused(void) {
  return (!playingMusic && currentTrack);
}

boolean AudioPro_FilePlayer::stopped(void) {
  return (!playingMusic && !currentTrack);
}

boolean AudioPro_FilePlayer::playMP3(const char *trackname) {
  if (!stopped()) {
    stopPlaying();  //必要，否则SD类得不到关闭，内存溢出
    delay(100);
  }
  if(!stopped()){
    return false;
  }

  // reset playback
  sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW);
  // resync
  sciWrite(VS1053_REG_WRAMADDR, 0x1e29);
  sciWrite(VS1053_REG_WRAM, 0);

  currentTrack = sd.open(trackname);
  if (!currentTrack) {
    return false;
  }

  // don't let the IRQ get triggered by accident here
  noInterrupts();

  if (!getAmplifier()) {
    setAmplifier(true);
    delay(200);
  }

  // As explained in datasheet, set twice 0 in REG_DECODETIME to set time back to 0
  sciWrite(VS1053_REG_DECODETIME, 0x00);
  sciWrite(VS1053_REG_DECODETIME, 0x00);

  playingMusic = true;

  // wait till its ready for data
  while (! readyForData() );

  // fill it up!
  while (playingMusic && readyForData())
    this->feedBuffer();

  //  Serial.println("Ready");
  // ok going forward, we can use the IRQ
  interrupts();
  
  return true;
}

boolean AudioPro_FilePlayer::playMP3(String trackname) {
  return playMP3(trackname.c_str());
}

boolean AudioPro_FilePlayer::playTrack(uint8_t trackNo){
  //a storage place for track names
  char trackName[] = "track001.mp3";
  uint8_t trackNumber = 1;

  //tack the number onto the rest of the filename
  sprintf(trackName, "track%03d.mp3", trackNo);

  //play the file
  return playMP3(trackName);
}

uint8_t AudioPro_FilePlayer::getMusicNum() {
  if (!staFile) {   //只执行一遍
    if (!paused() && !stopped()) {
      return 0;
    }

    numMusicFile = 0;

    char * filename = "TRACK001.mp3";
    
    File file2;
    if (sd.exists(sdData)) {
      sd.remove(sdData);
    }
    file2 = sd.open(sdData, FILE_WRITE);
    file2.close();
    if (!sd.exists(sdData)) return 0;
    file2 = sd.open(sdData, FILE_WRITE);
    if (!file2) return 0;
    
    File file;
    file = sd.open("/");
    if (!file) return 0;
    while (1) {
      File entry =  file.openNextFile(O_READ);
      if (! entry) {
        break;
      }
      filename = entry.name();
      if ( isFnMusic(filename) ) {
        file2.write(filename);
        file2.write('\n');
        numMusicFile++;
      }
      entry.close();
    }
    file.close();
    
    file2.close();
    staFile = true;
  }

  return numMusicFile;
}

String AudioPro_FilePlayer::getMusicName(uint8_t _FileNum){
  if(!paused() && !stopped()){
    return "PLAYING.ER";
  }
  if(_FileNum > getMusicNum() - 1){
    return "MUSICNUM.ER";    
  }
  
  String filename = "";
  filename.reserve(16);
  
  File file;
  file = SD.open(sdData); 
  if (file) {
    uint8_t _n = 0;
    // read from the file until there's nothing else in it:
    while (file.available()) {
      char _c = file.read();
      if (_c == '\n') {
        _n++;
        if (_n < _FileNum) {
          filename = "";
        }
        if(_n > _FileNum){
          break;
        }
      }
      else if (_n == _FileNum) {
          filename += _c;
      }
    }
    file.close();             //必须 解决SD库多次open以后缺失文件列表BUG
  }
  else {
    return "SDCARD.ER";
  }

  return filename;
}


void AudioPro_FilePlayer::feedBuffer(void) {
  noInterrupts();
  // Do not allow 2 copies of this code to run concurrently.
  // If an interrupt causes feedBuffer() to run while another
  // copy of feedBuffer() is already running in the main
  // program, havoc can occur.  "running" detects this state
  // and safely returns.
  if (feedBufferLock) {
    interrupts();
    return;  // return safely, before touching hardware!  :-)
  }
  feedBufferLock = true;
  interrupts();

  if (! playingMusic || ! currentTrack) {
    feedBufferLock = false;
    return; // paused or stopped
  }
  
  // Feed the hungry buffer! :)
  while (readyForData()) {
    // Read some audio data from the SD card file
    int bytesread = currentTrack.read(mp3buffer, VS1053_DATABUFFERLEN);

    if (bytesread == 0) {
      // must be at the end of the file, wrap it up!
      playingMusic = false;
      currentTrack.close();
      break;
    }
    playData(mp3buffer, bytesread);
  }
  feedBufferLock = false;
}

boolean AudioPro_FilePlayer::detachInterrupt(uint8_t type) {
  myself_sd = this;  // oy vey
/*  
  if (type == VS1053_TIMER0_DREQ) {
    OCR0A = 0xAF;
    TIMSK0 &= ~_BV(OCIE0A);
    return true;
  }
*/
  if (type == _dreq) {
    SPI.notUsingInterrupt(digitalPinToInterrupt(_dreq));
    detachInterrupt(digitalPinToInterrupt(_dreq));
    return true;
  }
  return false;
}

boolean AudioPro_FilePlayer::useInterrupt(uint8_t type) {
  if(myself) AudioPro::detachInterrupt();  //只允许一个中断实例
  myself_sd = this;  // oy vey
/*
  if (type == VS1053_TIMER0_DREQ) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    return true;
  }
*/
  if (type == _dreq) {
    SPI.usingInterrupt(digitalPinToInterrupt(_dreq));
    attachInterrupt(digitalPinToInterrupt(_dreq), feeder_sd, CHANGE);
    return true;
  }
  return false;
}

/***************************************************************/

AudioPro::AudioPro(uint8_t midi, uint8_t cs, uint8_t dcs, uint8_t dreq) {
  _midi = midi;
  _cs = cs;
  _dcs = dcs;
  _dreq = dreq;


  playingMusic = false;
  romTrack = false;
  romAddr = 0;
  romLen = 0;
  romLenCache = 0;
}

boolean AudioPro::useInterrupt(uint8_t type) {
  myself = this;  // oy vey
/*
  if (type == VS1053_TIMER0_DREQ) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    return true;
  }
*/
  if (type == _dreq) {
    SPI.usingInterrupt(digitalPinToInterrupt(_dreq));
    attachInterrupt(digitalPinToInterrupt(_dreq), feeder, CHANGE);
    return true;
  }
  return false;
}

boolean AudioPro::detachInterrupt(uint8_t type) {
  myself = this;  // oy vey
/*  
  if (type == VS1053_TIMER0_DREQ) {
    OCR0A = 0xAF;
    TIMSK0 &= ~_BV(OCIE0A);
    return true;
  }
*/
  if (type == _dreq) {
    SPI.notUsingInterrupt(digitalPinToInterrupt(_dreq));
    detachInterrupt(digitalPinToInterrupt(_dreq));
    return true;
  }
  return false;
}

void AudioPro::feedBuffer(void) {
  noInterrupts();
  // Do not allow 2 copies of this code to run concurrently.
  // If an interrupt causes feedBuffer() to run while another
  // copy of feedBuffer() is already running in the main
  // program, havoc can occur.  "running" detects this state
  // and safely returns.
  if (feedBufferLock) {
    interrupts();
    return;  // return safely, before touching hardware!  :-)
  }
  feedBufferLock = true;
  interrupts();

  if (! playingMusic || ! romTrack) {
    feedBufferLock = false;
    return; // paused or stopped
  }
  
  // Feed the hungry buffer! :)
  while (readyForData()) {
    int bytesread = min(romLen - romLenCache, VS1053_DATABUFFERLEN);
    for (uint16_t a = 0; a < bytesread; a++) {
      mp3buffer[a] = pgm_read_byte(romAddr + romLenCache);
      romLenCache++;
    }
    Serial.print("LEN:");Serial.print(bytesread);Serial.println(" ");
    if(romLenCache == romLen){
      //Serial.print(F("#END-"));Serial.print(F(" mp3buffer:["));
      //for (uint8_t i = 0; i < bytesread; i++) {
      //  Serial.print(F(" 0x"));Serial.print(mp3buffer[i],HEX);
      //}
      //Serial.print(F("] romLenCache:"));Serial.println(romLenCache);
      playingMusic = false;
      romTrack = false;
      romLenCache = 0;
      break;          
    }
    playData(mp3buffer, bytesread);
}
  feedBufferLock = false;
}

boolean AudioPro::paused(void) {
  return (!playingMusic && romTrack);
}

boolean AudioPro::stopped(void) {
  return (!playingMusic && !romTrack);
}

//------------------------------------------------------------------------------
/**
 * \brief flush the VSdsp buffer and cancel
 *
 * \param[in] mode is an enumerated value of flush_m
 *
 * Typically called after a filehandlers' stream has been stopped, as to
 * gracefully flush any buffer contents still playing. Along with issueing a
 * SM_CANCEL to the VSdsp's SCI_MODE register.

 * - post - will flush vx10xx's 2K buffer after cancelled, typically with stopping a file, to have immediate affect.
 * - pre  - will flush buffer prior to issuing cancel, typically to allow completion of file
 * - both - will flush before and after issuing cancel
 * - none - will just issue cancel. Not sure if this should be used. Such as in skipTo().
 *
 * \note if cancel fails the vs10xx will be reset and initialized to current values.
 */
void AudioPro::flushCancel(flush_m mode) {
  //Serial.println("\t flush");
  int8_t endFillByte = (int8_t) (ReadWRAM(VS1053_PARA_ENDFILL) & 0xFF);

  if((mode == post) || (mode == both)) {
    digitalWrite(_dcs, LOW); //Select Data
    for(int y = 0 ; y < 2052 ; y++) {
      while(!readyForData()); // wait until DREQ is or goes high
      spiwrite(endFillByte); // Send SPI byte
    }
    digitalWrite(_dcs, HIGH); //Deselect Data
  }

  for (int n = 0; n < 64 ; n++) {
    //sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_CANCEL); // old way of SCI_MODE WRITE.
    sciWrite(VS1053_REG_MODE, (sciRead(VS1053_REG_MODE) | VS1053_MODE_SM_CANCEL));

    digitalWrite(_dcs, LOW); //Select Data
    for(int y = 0 ; y < 32 ; y++) {
      while(!readyForData()); // wait until DREQ is or goes high
      spiwrite(endFillByte); // Send SPI byte
    }
    digitalWrite(_dcs, HIGH); //Deselect Data

    int cancel = sciRead(VS1053_REG_MODE) & VS1053_MODE_SM_CANCEL;
    if(cancel == 0) {
      // Cancel has succeeded.
      if((mode == pre) || (mode == both)) {
        digitalWrite(_dcs, LOW); //Select Data
        for(int y = 0 ; y < 2052 ; y++) {
          while(!readyForData()); // wait until DREQ is or goes high
          spiwrite(endFillByte); // Send SPI byte
        }
        digitalWrite(_dcs, HIGH); //Deselect Data
      }
      return;
    }
  }
  // Cancel has not succeeded.
  //Serial.println(F("Warning: VS10XX chip did not cancel, reseting chip!"));
  //sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_CANCEL); // old way of SCI_MODE WRITE.
  //sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_RESET); // old way of SCI_MODE WRITE.
  //softReset();
  sciWrite(VS1053_REG_MODE, (sciRead(VS1053_REG_MODE) | VS1053_MODE_SM_RESET));  // software reset. but vs_init will HW reset anyways.
  //begin(); // however, SFEMP3Shield::begin() is member function that does not exist statically.
}

void AudioPro::applyPatch(const uint16_t *patch, uint16_t patchsize) {
  uint16_t i = 0;

  //Serial.print("Patch size: "); Serial.println(patchsize);
  while ( i < patchsize ) {
    uint16_t addr, n, val;

    addr = pgm_read_word(patch++);
    n = pgm_read_word(patch++);
    i += 2;

    //Serial.println(addr, HEX);
    if (n & 0x8000U) { // RLE run, replicate n samples
      n &= 0x7FFF;
      val = pgm_read_word(patch++);
      i++;
      while (n--) {
        sciWrite(addr, val);
      }
    } else {           // Copy run, copy n samples
      while (n--) {
        val = pgm_read_word(patch++);
        i++;
        sciWrite(addr, val);
      }
    }
  }
}

void AudioPro::midiSetBank(uint8_t chan, uint8_t bank) {
  if (chan > 15 || bank > 127) return;

  uint8_t _c[] = {0, MIDI_CHAN_MSG | chan, 0, MIDI_CHAN_BANK, 0, bank};
  while (!digitalRead(_dreq));
  playData(_c, sizeof(_c));
}


void AudioPro::midiSetVolume(uint8_t chan, uint16_t vol) {
  if (chan > 15 || vol > 127) return;

  uint8_t _c[] = {0, MIDI_CHAN_MSG | chan, 0, MIDI_CHAN_VOLUME, 0, vol};
  while (!digitalRead(_dreq));
  playData(_c, sizeof(_c));
}

void AudioPro::midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15 || inst > 127) return;  // page 32 has instruments starting with 1 not 0 :(

  uint8_t _c[] = {0, MIDI_CHAN_PROGRAM | chan, 0, inst};
  while (!digitalRead(_dreq));
  playData(_c, sizeof(_c));
}

void AudioPro::noteOn(uint8_t chan, uint8_t n, uint8_t vol) {
  if (chan > 15 || n > 127 || vol > 127) return;

  uint8_t _c[] = {0, MIDI_NOTE_ON | chan, 0, n, 0, vol};
  while (!digitalRead(_dreq));
  playData(_c, sizeof(_c));
}


void AudioPro::noteOff(uint8_t chan, uint8_t n, uint8_t vol) {
  if (chan > 15 || n > 127 || vol > 127) return;

  uint8_t _c[] = {0, MIDI_NOTE_OFF | chan, 0, n, 0, vol};
  while (!digitalRead(_dreq));
  playData(_c, sizeof(_c));
}


boolean AudioPro::readyForData(void) {
  return digitalRead(_dreq);
}

void AudioPro::playData(uint8_t *buffer, uint8_t buffsiz) {
  SPI.beginTransaction(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);
  for (uint8_t i = 0; i < buffsiz; i++) {
    spiwrite(buffer[i]);
  }
  digitalWrite(_dcs, HIGH);
  SPI.endTransaction();
}


void AudioPro::playBuffer(uint8_t *buffer, size_t buffsiz) {
  SPI.beginTransaction(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);
  while ( buffsiz ) {
    while (!readyForData());
    delayMicroseconds(3);
    size_t chunk_length = min(buffsiz, VS1053_DATABUFFERLEN);
    buffsiz -= chunk_length;
    while ( chunk_length-- ) spiwrite(*buffer++);
  }
  digitalWrite(_dcs, HIGH);
  SPI.endTransaction();
}


boolean AudioPro::playROM(const uint8_t *_buffer, uint32_t _len) {
  //Serial.print(F("\t after Status: 0x"));Serial.println(getStatus(),HEX);
  if (getStatus() != 0x40) {
    //feedBufferLock = false;
    //Serial.println("\t #flush_cancel");
    stopPlaying();
    flushCancel(both);
    //Serial.print(F("\t before Status: 0x"));Serial.println(getStatus(),HEX);
    if(getStatus() != 0x40){
      sciWrite(VS1053_REG_STATUS,(uint16_t)0x40);
      if(getStatus() != 0x40){
        //Serial.println(F("\t Status ERROR"));
        return false;
      }
    }
  }

  // reset playback
  sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW);
  // resync
  sciWrite(VS1053_REG_WRAMADDR, 0x1e29);
  sciWrite(VS1053_REG_WRAM, 0);

  romAddr = _buffer;
  romLen = _len;
  romLenCache = 0;
  romTrack = true;
  playingMusic = true;
  uint32_t _romLen = sizeof(_buffer);
  //Serial.print("romAddr:");Serial.print(romAddr);Serial.print(" romLen");Serial.println(_romLen);
  
  if (!getAmplifier()) {
    setAmplifier(true);
    delay(200);
  }
  
  noInterrupts();
  // As explained in datasheet, set twice 0 in REG_DECODETIME to set time back to 0
  sciWrite(VS1053_REG_DECODETIME, 0x00);
  sciWrite(VS1053_REG_DECODETIME, 0x00);

  // wait till its ready for data
  while (! readyForData() );

  // fill it up!
  while (playingMusic && readyForData())
    this->feedBuffer();

  //Serial.println("Ready");
  interrupts();
  return true;  
}

void AudioPro::pausePlaying(boolean pause) {
  if (pause) {
    //setAmplifier(false);
    playingMusic = false;
  }
  else {
    //setAmplifier(true);
    playingMusic = true;
    this->feedBuffer();
  }
}

uint16_t AudioPro::getVolume() {
  noInterrupts(); //cli();
  uint16_t _vol = sciRead(VS1053_REG_VOLUME);
  if (_vol == 0) _vol = 512;
  interrupts();  //sei();
  return _vol;
}

void AudioPro::setVolume(uint8_t left, uint8_t right) {
  if(left > 127 || right > 127) return;
  noInterrupts(); //cli();
  sciWrite(VS1053_REG_VOLUME, left, right);
  interrupts();  //sei();
}

void AudioPro::setVolume(uint8_t left_right) {
  setVolume(left_right, left_right);
}

uint8_t AudioPro::volumeUp() {
  union twobyte _vol; // create key_command existing variable that can be both word and double byte of left and right.
  _vol.word = getVolume(); // returns a double uint8_t of Left and Right packed into int16_t

  _vol.byte[1] -= 2; // keep it simpler with whole dB's
  if (_vol.byte[1] < 2) { // range check
    _vol.byte[1] = 2;
  }

  // push byte[1] into both left and right assuming equal balance.
  setVolume(_vol.byte[1]); // commit new volume
  return _vol.byte[1];
}

uint8_t AudioPro::volumeDown() {
  union twobyte _vol; // create key_command existing variable that can be both word and double byte of left and right.
  _vol.word = getVolume(); // returns a double uint8_t of Left and Right packed into int16_t

  _vol.byte[1] += 2; // keep it simpler with whole dB's
  if (_vol.byte[1] > 254) { // range check
    _vol.byte[1] = 254;
  }

  // push byte[1] into both left and right assuming equal balance.
  setVolume(_vol.byte[1]); // commit new volume
  return _vol.byte[1];
}

uint16_t AudioPro::getPlaySpeed() {
  uint16_t MP3playspeed = ReadWRAM(VS1053_PARA_PLAYSPEED);
  return MP3playspeed;
}

void AudioPro::setPlaySpeed(uint16_t data) {
  WriteWRAM(VS1053_PARA_PLAYSPEED, data);
}

uint16_t AudioPro::decodeTime() {
  noInterrupts(); //cli();
  uint16_t t = sciRead(VS1053_REG_DECODETIME);
  interrupts(); //sei();
  return t;
}

uint16_t AudioPro::getMonoMode() {
  uint16_t result = (ReadWRAM(VS1053_PARA_MONOOUTPUT) & 0x0001);
  return result;
}

void AudioPro::setMonoMode(uint16_t StereoMode) {
  uint16_t data = (ReadWRAM(VS1053_PARA_MONOOUTPUT) & ~0x0001); // preserve other bits
  WriteWRAM(0x1e09, (StereoMode | (data & 0x0001)));
}

uint8_t AudioPro::getDifferentialOutput() {
  uint8_t result = 0;
  uint16_t MP3SCI_MODE = sciRead(VS1053_REG_MODE);

  if (MP3SCI_MODE & VS1053_MODE_SM_DIFF) {
    result = 1;
  }
  return result;
}

void AudioPro::setDifferentialOutput(uint16_t DiffMode) {
  uint16_t MP3SCI_MODE = sciRead(VS1053_REG_MODE);

  if (DiffMode) {
    MP3SCI_MODE |=  VS1053_MODE_SM_DIFF;
  } else {
    MP3SCI_MODE &= ~VS1053_MODE_SM_DIFF;
  }
  sciWrite(VS1053_REG_MODE, MP3SCI_MODE);
}

void AudioPro::softReset(void) {
  sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_RESET);
  delay(100);
}

void AudioPro::reset() {
  // TODO: http://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a3
  digitalWrite(_cs, HIGH);
  digitalWrite(_dcs, HIGH);
  delay(100);
  softReset();
  delay(100);

  sciWrite(VS1053_REG_CLOCKF, 0x6000);

  setVolume(VS1053_DEFAULT_VOLUME, VS1053_DEFAULT_VOLUME);
}

void AudioPro::end() {
  stopPlaying();
  //GPIO_digitalWrite(4,HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_dcs, HIGH);

  setAmplifier(false);
}

uint8_t AudioPro::begin(void) {
  pinMode(_midi, INPUT_PULLUP);
  delay(100);

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  pinMode(_dcs, OUTPUT);
  digitalWrite(_dcs, HIGH);
  pinMode(_dreq, INPUT);
  delay(100);

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);

  reset();

  setAmplifier(true);

  return (sciRead(VS1053_REG_STATUS) >> 4) & 0x0F;
}

uint16_t AudioPro::recordedWordsWaiting(void) {
  return sciRead(VS1053_REG_HDAT1);
}

uint16_t AudioPro::recordedReadWord(void) {
  return sciRead(VS1053_REG_HDAT0);
}

void AudioPro::stopPlaying(void) {
  setAmplifier(false);
  // cancel all playback
  sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_CANCEL);
}

uint16_t AudioPro::getStatus(){
  return sciRead(VS1053_REG_STATUS);
}

void AudioPro::stopRecordOgg(void) {
  sciWrite(VS1053_SCI_AICTRL3, 1);
}

void AudioPro::startRecordOgg(boolean mic) {
  /* Set VS1053 mode bits as instructed in the VS1053b Ogg Vorbis Encoder
     manual. Note: for microphone input, leave SMF_LINE1 unset! */
  if (mic) {
    sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_ADPCM | VS1053_MODE_SM_SDINEW);
  } else {
    sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_LINE1 |
             VS1053_MODE_SM_ADPCM | VS1053_MODE_SM_SDINEW);
  }
  sciWrite(VS1053_SCI_AICTRL0, 1024);
  /* Rec level: 1024 = 1. If 0, use AGC */
  sciWrite(VS1053_SCI_AICTRL1, 1024);
  /* Maximum AGC level: 1024 = 1. Only used if SCI_AICTRL1 is set to 0. */
  sciWrite(VS1053_SCI_AICTRL2, 0);
  /* Miscellaneous bits that also must be set before recording. */
  sciWrite(VS1053_SCI_AICTRL3, 0);

  sciWrite(VS1053_SCI_AIADDR, 0x34);
  delay(1);
  while (! readyForData() );
}

void AudioPro::setAmplifier(boolean sta) {
  //配置输出模式
  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_DDR);
  uint16_t ddr = sciRead(VS1053_REG_WRAM);
  ddr |= _BV(4);
  WriteWRAM(VS1053_GPIO_DDR, ddr);

  //设置GPIO4开关，低电平有效
  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_ODATA);
  uint16_t pins = sciRead(VS1053_REG_WRAM);

  if (sta)
    pins &= ~_BV(4);
  else
    pins |= _BV(4);

  WriteWRAM(VS1053_GPIO_ODATA, pins);
}

boolean AudioPro::getAmplifier() {
  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_IDATA);
  uint16_t val = sciRead(VS1053_REG_WRAM);
  if (val & _BV(4)) return false;
  return true;
}

uint16_t AudioPro::sciRead(uint8_t addr) {
  uint16_t data;

  SPI.beginTransaction(VS1053_CONTROL_SPI_SETTING);
  digitalWrite(_cs, LOW);
  spiwrite(VS1053_SCI_READ);
  spiwrite(addr);
  delayMicroseconds(10);
  data = spiread();
  data <<= 8;
  data |= spiread();
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();

  return data;
}


void AudioPro::sciWrite(uint8_t addr, uint8_t data_H, uint8_t data_L) {
  SPI.beginTransaction(VS1053_CONTROL_SPI_SETTING);
  digitalWrite(_cs, LOW);
  spiwrite(VS1053_SCI_WRITE);
  spiwrite(addr);
  spiwrite(data_H);
  spiwrite(data_L);
  digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}


void AudioPro::sciWrite(uint8_t addr, uint16_t data) {
  union twobyte val;
  val.word = data;
  sciWrite(addr, val.byte[1], val.byte[0]);
}


uint8_t AudioPro::spiread(void) {
  return SPI.transfer(0x00);
}

void AudioPro::spiwrite(uint8_t c) {
  SPI.transfer(c);
}

//------------------------------------------------------------------------------
/**
   \brief Read a VS10xx WRAM Location

   \param[in] addressbyte of the VSdsp's WRAM to be read
   \return result read from the WRAM

   Function to communicate to the VSdsp's registers, indirectly accessing the WRAM.
   As per data sheet the result is read back twice to verify. As it is not buffered.
*/
uint16_t AudioPro::ReadWRAM (uint16_t addressbyte) {
  uint16_t tmp1, tmp2;

  //Set SPI bus for write
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);

  sciWrite(VS1053_REG_WRAMADDR, addressbyte);

  tmp1 = sciRead(VS1053_REG_WRAM);

  sciWrite(VS1053_REG_WRAMADDR, addressbyte);
  tmp2 = sciRead(VS1053_REG_WRAM);
  if (tmp1 == tmp2) return tmp1;

  sciWrite(VS1053_REG_WRAMADDR, addressbyte);
  tmp2 = sciRead(VS1053_REG_WRAM);
  if (tmp1 == tmp2) return tmp1;

  sciWrite(VS1053_REG_WRAMADDR, addressbyte);
  tmp2 = sciRead(VS1053_REG_WRAM);
  if (tmp1 == tmp2) return tmp1;

  return tmp1;
}

//------------------------------------------------------------------------------
/**
   \brief Write a VS10xx WRAM Location

   \param[in] addressbyte of the VSdsp's WRAM to be read
   \param[in] data written to the VSdsp's WRAM

   Function to communicate to the VSdsp's registers, indirectly accessing the WRAM.
*/
//Write the 16-bit value of a VS10xx WRAM location
void AudioPro::WriteWRAM(uint16_t addressbyte, uint16_t data) {
  sciWrite(VS1053_REG_WRAMADDR, addressbyte);
  sciWrite(VS1053_REG_WRAM, data);
}

void AudioPro::sineTest(uint8_t n, uint16_t ms) {
  reset();

  uint16_t mode = sciRead(VS1053_REG_MODE);
  mode |= 0x0020;
  sciWrite(VS1053_REG_MODE, mode);

  while (!digitalRead(_dreq));
  //  delay(10);

  SPI.beginTransaction(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);
  spiwrite(0x53);
  spiwrite(0xEF);
  spiwrite(0x6E);
  spiwrite(n);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  digitalWrite(_dcs, HIGH);
  SPI.endTransaction();

  delay(ms);

  SPI.beginTransaction(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);
  spiwrite(0x45);
  spiwrite(0x78);
  spiwrite(0x69);
  spiwrite(0x74);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  digitalWrite(_dcs, HIGH);
  SPI.endTransaction();
}


/**
   \brief is the filename music

   \param[in] filename inspects the end of the filename to be of the extension types
              that VS10xx can decode.

   \return boolean true indicating that it is music
*/
bool isFnMusic(char* filename) {
  int8_t len = strlen(filename);
  bool result;
  if (  strstr(strlwr(filename + (len - 4)), ".mp3")
        || strstr(strlwr(filename + (len - 4)), ".aac")
        || strstr(strlwr(filename + (len - 4)), ".wma")
        || strstr(strlwr(filename + (len - 4)), ".wav")
        || strstr(strlwr(filename + (len - 4)), ".fla")
        || strstr(strlwr(filename + (len - 4)), ".mid")
     ) {
    result = true;
  } else {
    result = false;
  }
  return result;
}