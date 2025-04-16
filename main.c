#include <stdio.h>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"

#define MIDI_CHANNEL 0  // Channel 1 (zero-indexed)
#define NOTE_C3 48

#define MIDI_CC_VOLUME     7
#define MIDI_CC_MODULATION 1

void example_task(void);

int main() {
    board_init();
    tusb_init();

    while (1) {
        tud_task();       // Required for USB operation
        example_task();   // MIDI loop
    }
}

void example_task(void) {
    static uint32_t last_time = 0;
    static bool note_on = false;

    if (board_millis() - last_time < 1000) return;
    last_time = board_millis();

    if (!note_on) {
        send_midi_note_on(MIDI_CHANNEL, NOTE_C3, 100);
        note_on = true;
    } else {
        send_midi_note_off(MIDI_CHANNEL, NOTE_C3, 0);
        note_on = false;
    }

    send_midi_control_change(MIDI_CHANNEL, MIDI_CC_VOLUME, 100);
    send_midi_control_change(MIDI_CHANNEL, MIDI_CC_MODULATION, 50);
    send_midi_pitch_bend(MIDI_CHANNEL, 0);
}

