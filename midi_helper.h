#ifndef MIDI_HELPER_H
#define MIDI_HELPER_H

#include <stdint.h>

void send_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity);
void send_midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity);
void send_midi_control_change(uint8_t channel, uint8_t control, uint8_t value);
void send_midi_pitch_bend(uint8_t channel, int16_t bend);

#endif
