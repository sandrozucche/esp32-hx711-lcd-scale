#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "lcd_i2c.h"
#include "rom/ets_sys.h"

#define LCD_RS 0x01
#define LCD_EN 0x04
#define LCD_BL 0x08
#define I2C_MASTER_NUM I2C_NUM_0

static uint8_t lcd_backlight = LCD_BL;

static esp_err_t lcd_write_pcf(uint8_t data) {
    return i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &data, 1, pdMS_TO_TICKS(100));
}

static void lcd_pulse_enable(uint8_t data) {
    lcd_write_pcf(data | LCD_EN | lcd_backlight);
    ets_delay_us(1);
    lcd_write_pcf((data & ~LCD_EN) | lcd_backlight);
    ets_delay_us(50);
}

static void lcd_write4bits(uint8_t nibble, uint8_t mode) {
    uint8_t data = mode ? LCD_RS : 0;
    if (nibble & 0x01) data |= (1 << 4);
    if (nibble & 0x02) data |= (1 << 5);
    if (nibble & 0x04) data |= (1 << 6);
    if (nibble & 0x08) data |= (1 << 7);
    lcd_write_pcf(data | lcd_backlight);
    lcd_pulse_enable(data);
}

void lcd_send_cmd(uint8_t cmd) {
    lcd_write4bits((cmd >> 4) & 0x0F, 0);
    lcd_write4bits(cmd & 0x0F, 0);
}

void lcd_send_data(uint8_t data) {
    lcd_write4bits((data >> 4) & 0x0F, 1);
    lcd_write4bits(data & 0x0F, 1);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) lcd_send_data((uint8_t)*str++);
}

void lcd_clear(void) {
    lcd_send_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_clear_line(uint8_t row) {
    lcd_set_cursor(0, row);
    lcd_print("                    ");
    lcd_set_cursor(0, row);
}

esp_err_t lcd_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    vTaskDelay(pdMS_TO_TICKS(50));
    lcd_write4bits(0x03, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_write4bits(0x03, 0);
    ets_delay_us(150);
    lcd_write4bits(0x03, 0);
    lcd_write4bits(0x02, 0);
    lcd_send_cmd(0x28);
    lcd_send_cmd(0x0C);
    lcd_send_cmd(0x06);
    lcd_clear();
    return ESP_OK;
}