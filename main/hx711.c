#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hx711.h"
#include "rom/ets_sys.h"

static int32_t scale_offset = 0;

void hx711_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << HX711_DT_GPIO) | (1ULL << HX711_SCK_GPIO),
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gpio_set_direction(HX711_DT_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(HX711_SCK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(HX711_SCK_GPIO, 0);
}

bool hx711_is_ready(void) {
    return gpio_get_level(HX711_DT_GPIO) == 0;
}

static int32_t hx711_read_raw(void) {
    int timeout = 100;
    while (!hx711_is_ready() && timeout-- > 0) vTaskDelay(pdMS_TO_TICKS(1));
    if (timeout <= 0) return 0;

    int32_t value = 0;
    for (int i = 0; i < 24; i++) {
        gpio_set_level(HX711_SCK_GPIO, 1);
        ets_delay_us(1);
        value = (value << 1) | gpio_get_level(HX711_DT_GPIO);
        gpio_set_level(HX711_SCK_GPIO, 0);
        ets_delay_us(1);
    }
    gpio_set_level(HX711_SCK_GPIO, 1);
    ets_delay_us(1);
    gpio_set_level(HX711_SCK_GPIO, 0);

    if (value & 0x800000) value |= ~0xFFFFFF;
    return value;
}

int32_t hx711_read_average(int times) {
    int64_t sum = 0;
    for (int i = 0; i < times; i++) {
        sum += hx711_read_raw();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return (int32_t)(sum / times);
}

void hx711_tare(int samples) {
    scale_offset = hx711_read_average(samples);
}

float hx711_get_units(int samples, float calibration_factor) {
    int32_t raw = hx711_read_average(samples);
    return ((float)(raw - scale_offset)) / calibration_factor;
}