#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/i2c.h"
#include "ads1115.h"
#include "mpu6050.h"
#include "ssd1306.h"

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

#define ADS1115_1_ADDR 0x48
#define ADS1115_2_ADDR 0x49
#define MPU6050_ADDR   0x68

#define BUTTON_PINS {2, 3, 4, 5}

void init_buttons() {
    int pins[] = BUTTON_PINS;
    for (int i = 0; i < 4; i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
    }
}

int main() {
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // I2C setup
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Init hardware
    init_buttons();

    ads1115_adc_t adc1, adc2;
    ads1115_init(I2C_PORT, ADS1115_1_ADDR, &adc1);
    ads1115_init(I2C_PORT, ADS1115_2_ADDR, &adc2);

    mpu6050_t mpu = mpu6050_init(I2C_PORT, MPU6050_ADDR);
    mpu6050_begin(&mpu);

    while (true) {
        // --- Read from ADS1115s ---
        uint16_t adc_vals[8];
        for (int i = 0; i < 4; i++) {
            ads1115_set_input_mux((enum ads1115_mux_t)(0x04 + i), &adc1); // AIN0–3
            sleep_ms(2); // give time to settle
            ads1115_read_adc(&adc_vals[i], &adc1);
        }

        for (int i = 0; i < 4; i++) {
            ads1115_set_input_mux((enum ads1115_mux_t)(0x04 + i), &adc2);
            sleep_ms(2);
            ads1115_read_adc(&adc_vals[i + 4], &adc2);
        }

        // --- Read MPU6050 ---
        mpu6050_event(&mpu);

        // --- Read buttons ---
        int pins[] = BUTTON_PINS;
        bool buttons[4];
        for (int i = 0; i < 4; i++) {
            buttons[i] = gpio_get(pins[i]);
        }

        // --- Print output ---
        printf("ADC: %d %d %d %d | %d %d %d %d\n",
               adc_vals[0], adc_vals[1], adc_vals[2], adc_vals[3],
               adc_vals[4], adc_vals[5], adc_vals[6], adc_vals[7]);

        printf("MPU: Accel [%d, %d, %d], Gyro [%d, %d, %d], Temp: %.2f°C\n",
               mpu.ra.x, mpu.ra.y, mpu.ra.z,
               mpu.rg.x, mpu.rg.y, mpu.rg.z,
               mpu.temperature);

        printf("Buttons: %d %d %d %d\n\n",
               buttons[0], buttons[1], buttons[2], buttons[3]);

        sleep_ms(200);
    }
}
