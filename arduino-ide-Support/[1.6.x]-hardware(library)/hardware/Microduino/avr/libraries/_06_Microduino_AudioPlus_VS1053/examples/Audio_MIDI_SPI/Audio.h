#include "AudioDef.h"
#include <Adafruit_VS1053.h>
Adafruit_VS1053 musicPlayer = Adafruit_VS1053(VS_XRESET, VS_XCS, VS_XDCS, VS_DREQ);


void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15 || inst > 127) return;  // page 32 has instruments starting with 1 not 0 :(

  uint8_t _c[] = {0, MIDI_CHAN_PROGRAM | chan, 0, inst};
  while (!musicPlayer.readyForData());
  musicPlayer.playData(_c, sizeof(_c));
}

void midiSetChannelVolume(uint8_t chan, uint16_t vol) {
  if (chan > 15 || vol > 127) return;

  uint8_t _c[] = {0, MIDI_CHAN_MSG | chan, 0, MIDI_CHAN_VOLUME, 0, vol};
  while (!musicPlayer.readyForData());
  musicPlayer.playData(_c, sizeof(_c));
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15 || bank > 127) return;

  uint8_t _c[] = {0, MIDI_CHAN_MSG | chan, 0, MIDI_CHAN_BANK, 0, bank};
  while (!musicPlayer.readyForData());
  musicPlayer.playData(_c, sizeof(_c));
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vol) {
  if (chan > 15 || n > 127 || vol > 127) return;

  uint8_t _c[] = {0, MIDI_NOTE_ON | chan, 0, n, 0, vol};
  while (!musicPlayer.readyForData());
  musicPlayer.playData(_c, sizeof(_c));
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vol) {
  if (chan > 15 || n > 127 || vol > 127) return;

  uint8_t _c[] = {0, MIDI_NOTE_OFF | chan, 0, n, 0, vol};
  while (!musicPlayer.readyForData());
  musicPlayer.playData(_c, sizeof(_c));
}
