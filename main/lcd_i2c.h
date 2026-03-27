#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "esp_err.h"

// Configurações do LCD
#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

// Pinos I2C
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22

esp_err_t lcd_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_clear(void);
void lcd_clear_line(uint8_t row);

#endif