#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "hx711.h"
#include "lcd_i2c.h"

static const char *TAG = "BALANCA_PRINCIPAL";
static float calibration_factor = -7050.0f;

void app_main(void) {
    // Inicialização
    ESP_ERROR_CHECK(lcd_init());
    hx711_init();

    lcd_set_cursor(0, 0);
    lcd_print("Iniciando Sistema...");
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Tara
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Remova o peso...");
    lcd_set_cursor(0, 1);
    lcd_print("Fazendo tara...");
    hx711_tare(20);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Balança Pronta!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    lcd_clear();

    char buffer[32];
    while (1) {
        float peso = hx711_get_units(10, calibration_factor);
        if (peso < 0) peso = 0.0f;

        ESP_LOGI(TAG, "Leitura: %.1f g", peso);

        lcd_set_cursor(0, 0);
        lcd_print("Monitor de Carga");
        
        lcd_clear_line(1);
        snprintf(buffer, sizeof(buffer), "Peso: %.1f g", peso);
        lcd_set_cursor(0, 1);
        lcd_print(buffer);

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}