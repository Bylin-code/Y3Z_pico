#include <stdio.h>
#include "pico/stdlib.h"

#define BTN_1 2
#define BTN_2 3
#define BTN_3 4
#define BTN_4 5

void setup_buttons() {
    gpio_init(BTN_1);
    gpio_set_dir(BTN_1, GPIO_IN);
    gpio_pull_up(BTN_1);  // internal pull-up

    gpio_init(BTN_2);
    gpio_set_dir(BTN_2, GPIO_IN);
    gpio_pull_up(BTN_2);

    gpio_init(BTN_3);
    gpio_set_dir(BTN_3, GPIO_IN);
    gpio_pull_up(BTN_3);

    gpio_init(BTN_4);
    gpio_set_dir(BTN_4, GPIO_IN);
    gpio_pull_up(BTN_4);
}

int main() {
    stdio_init_all();
    setup_buttons();

    while (true) {
        bool btn1 = !gpio_get(BTN_1); // invert logic: pressed = 1
        bool btn2 = !gpio_get(BTN_2);
        bool btn3 = !gpio_get(BTN_3);
        bool btn4 = !gpio_get(BTN_4);

        printf("Buttons: [%d %d %d %d]\n", btn1, btn2, btn3, btn4);

        sleep_ms(100); // poll every 100 ms
    }
}
