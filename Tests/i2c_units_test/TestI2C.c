#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

// I2C setup
#define I2C_PORT i2c0
#define SDA_PIN 20
#define SCL_PIN 21

// I2C addresses
#define ADS1_ADDR 0x48
#define ADS2_ADDR 0x49
#define MPU_ADDR  0x68
#define OLED_ADDR 0x3C

// ADS1115 registers
#define ADS_CONVERT 0x00
#define ADS_CONFIG  0x01

// MPU6050 registers
#define MPU_PWR_MGMT_1     0x6B
#define MPU_ACCEL_XOUT_H   0x3B

// OLED instance
static ssd1306_t oled;

// Read one channel from ADS1115 at 860 SPS
int16_t read_ads(uint8_t addr, uint8_t ch) {
    uint16_t config = 0x8000 | ((0x04 + ch) << 12) | 0x0200 | 0x0100 | 0x00E0 | 0x0003;
    uint8_t cfg[] = {ADS_CONFIG, config >> 8, config & 0xFF};
    i2c_write_blocking(I2C_PORT, addr, cfg, 3, false);
    sleep_us(1200); // Wait for conversion (~1.16ms)
    uint8_t reg = ADS_CONVERT, res[2];
    i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, addr, res, 2, false);
    return (res[0] << 8) | res[1];
}

// Initialize MPU6050
void mpu6050_init() {
    uint8_t buf[] = {MPU_PWR_MGMT_1, 0x00};
    i2c_write_blocking(I2C_PORT, MPU_ADDR, buf, 2, false);
}

// Read only accelerometer data from MPU6050
void mpu6050_read(int16_t *accel, int16_t *unused) {
    uint8_t reg = MPU_ACCEL_XOUT_H;
    uint8_t data[14];
    i2c_write_blocking(I2C_PORT, MPU_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU_ADDR, data, 14, false);
    for (int i = 0; i < 3; i++) {
        accel[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
}

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    mpu6050_init();
    ssd1306_init(&oled, 128, 64, OLED_ADDR, I2C_PORT);

    int16_t accel[3];
    char line[22];

    while (true) {
        ssd1306_clear(&oled);

        // ADS1115 A1
        snprintf(line, sizeof(line), "A1: %-5d %-5d", read_ads(ADS1_ADDR, 0), read_ads(ADS1_ADDR, 1));
        ssd1306_draw_string(&oled, 0, 0, 1, line);
        snprintf(line, sizeof(line), "    %-5d %-5d", read_ads(ADS1_ADDR, 2), read_ads(ADS1_ADDR, 3));
        ssd1306_draw_string(&oled, 0, 8, 1, line);

        // ADS1115 A2
        snprintf(line, sizeof(line), "A2: %-5d %-5d", read_ads(ADS2_ADDR, 0), read_ads(ADS2_ADDR, 1));
        ssd1306_draw_string(&oled, 0, 16, 1, line);
        snprintf(line, sizeof(line), "    %-5d %-5d", read_ads(ADS2_ADDR, 2), read_ads(ADS2_ADDR, 3));
        ssd1306_draw_string(&oled, 0, 24, 1, line);

        // MPU6050 Accelerometer
        mpu6050_read(accel, NULL);
        snprintf(line, sizeof(line), "AX: %-6d AY: %-6d", accel[0], accel[1]);
        ssd1306_draw_string(&oled, 0, 36, 1, line);
        snprintf(line, sizeof(line), "AZ: %-6d", accel[2]);
        ssd1306_draw_string(&oled, 0, 44, 1, line);

        ssd1306_show(&oled);

        sleep_ms(10);  // Fast refresh
    }
}
