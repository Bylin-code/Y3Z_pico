#include "midi_helper.h"
#include "tusb.h"

void send_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t msg[3] = { 0x90 | (channel & 0x0F), note, velocity };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

void send_midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity) {
    uint8_t msg[3] = { 0x80 | (channel & 0x0F), note, velocity };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

void send_midi_control_change(uint8_t channel, uint8_t control, uint8_t value) {
    uint8_t msg[3] = { 0xB0 | (channel & 0x0F), control, value };
    tud_midi_n_stream_write(0, 0, msg, 3);
}

void send_midi_pitch_bend(uint8_t channel, int16_t bend) {
    if (bend < -8192) bend = -8192;
    if (bend > 8191)  bend = 8191;

    uint16_t value = (uint16_t)(bend + 8192);
    uint8_t lsb = value & 0x7F;
    uint8_t msb = (value >> 7) & 0x7F;

    uint8_t msg[3] = { 0xE0 | (channel & 0x0F), lsb, msb };
    tud_midi_n_stream_write(0, 0, msg, 3);
}
