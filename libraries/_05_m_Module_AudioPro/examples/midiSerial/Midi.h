#if defined (__AVR__)
#include <SoftwareSerial.h>
SoftwareSerial VS1053_MIDI(-1, VS1053_PIN_MIDI);
#endif

//CoreESP UART Port: [RX,TX]
#if defined(ESP32)
HardwareSerial VS1053_MIDI(1);
#endif


void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15 || inst > 127) return;  // page 32 has instruments starting with 1 not 0 :(

  VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);
  VS1053_MIDI.write(inst);
}

void midiSetChannelVolume(uint8_t chan, uint16_t vol) {
  if (chan > 15 || vol > 127) return;

  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write(MIDI_CHAN_VOLUME);
  VS1053_MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15 || bank > 127) return;

  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
  VS1053_MIDI.write(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vol) {
  if (chan > 15 || n > 127 || vol > 127) return;

  VS1053_MIDI.write(MIDI_NOTE_ON | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vol);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vol) {
  if (chan > 15 || n > 127 || vol > 127) return;

  VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vol);
}