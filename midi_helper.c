#include "midi_helper.h"
#include "tusb.h"

// ============================
// Note On / Off
// ============================
void send_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t msg[3] = { 0x90 | (channel & 0x0F), note, velocity };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

void send_midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t msg[3] = { 0x80 | (channel & 0x0F), note, velocity };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

// ============================
// Control Change
// ============================
void send_midi_control_change(uint8_t channel, uint8_t control, uint8_t value) {
    uint8_t msg[3] = { 0xB0 | (channel & 0x0F), control, value };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

// ============================
// Pitch Bend
// ============================
void send_midi_pitch_bend(uint8_t channel, int16_t bend) {
    if (bend < -8192) bend = -8192;
    if (bend > 8191)  bend = 8191;

    uint16_t value = bend + 8192;
    uint8_t lsb = value & 0x7F;
    uint8_t msb = (value >> 7) & 0x7F;

    uint8_t msg[3] = { 0xE0 | (channel & 0x0F), lsb, msb };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

// ============================
// Aftertouch (Channel + Polyphonic)
// ============================
void send_midi_channel_pressure(uint8_t channel, uint8_t pressure) {
    uint8_t msg[2] = { 0xD0 | (channel & 0x0F), pressure };
    tud_midi_n_stream_write(0, 0, msg, 2);
}

void send_midi_poly_aftertouch(uint8_t channel, uint8_t note, uint8_t pressure) {
    uint8_t msg[3] = { 0xA0 | (channel & 0x0F), note, pressure };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

// ============================
// Program Change
// ============================
void send_midi_program_change(uint8_t channel, uint8_t program) {
    uint8_t msg[2] = { 0xC0 | (channel & 0x0F), program };
    tud_midi_n_stream_write(0, 0, msg, 2);
}

// ============================
// System Common Messages
// ============================
void send_midi_song_select(uint8_t song_number) {
    uint8_t msg[2] = { 0xF3, song_number };
    tud_midi_n_stream_write(0, 0, msg, 2);
}

void send_midi_tune_request(void) {
    uint8_t msg[1] = { 0xF6 };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

// ============================
// Real-Time Messages
// ============================
void send_midi_start(void) {
    uint8_t msg[1] = { 0xFA };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

void send_midi_continue(void) {
    uint8_t msg[1] = { 0xFB };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

void send_midi_stop(void) {
    uint8_t msg[1] = { 0xFC };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

void send_midi_timing_clock(void) {
    uint8_t msg[1] = { 0xF8 };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

void send_midi_active_sensing(void) {
    uint8_t msg[1] = { 0xFE };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

void send_midi_system_reset(void) {
    uint8_t msg[1] = { 0xFF };
    tud_midi_n_stream_write(0, 0, msg, 1);
}

// ============================
// SysEx (System Exclusive)
// ============================
void send_midi_sysex(uint8_t* data, uint16_t length) {
    // User must pass a full SysEx message (starts with 0xF0, ends with 0xF7)
    tud_midi_n_stream_write(0, 0, data, length);
}
