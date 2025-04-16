#ifndef MIDI_HELPER_H
#define MIDI_HELPER_H

#include <stdint.h>

// ====== Core Note Control ======
void send_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity);
void send_midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity);

// ====== Parameter Control (CC, Volume, Modulation, Pan) ======
void send_midi_control_change(uint8_t channel, uint8_t control, uint8_t value);

// ====== Pitch and Expression ======
void send_midi_pitch_bend(uint8_t channel, int16_t bend);  // -8192 to +8191
void send_midi_channel_pressure(uint8_t channel, uint8_t pressure);
void send_midi_poly_aftertouch(uint8_t channel, uint8_t note, uint8_t pressure);

// ====== Program and Instrument Control ======
void send_midi_program_change(uint8_t channel, uint8_t program);

// ====== System Common Messages ======
void send_midi_song_select(uint8_t song_number);
void send_midi_tune_request(void);

// ====== System Real-Time Messages ======
void send_midi_start(void);
void send_midi_continue(void);
void send_midi_stop(void);
void send_midi_timing_clock(void);
void send_midi_active_sensing(void);
void send_midi_system_reset(void);

// ====== SysEx (Raw) ======
void send_midi_sysex(uint8_t* data, uint16_t length);

#endif // MIDI_HELPER_H
