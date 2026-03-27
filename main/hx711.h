#ifndef HX711_H
#define HX711_H

#include <stdbool.h>
#include <stdint.h>

// Definições de Pinos
#define HX711_DT_GPIO 13
#define HX711_SCK_GPIO 14

void hx711_init(void);
bool hx711_is_ready(void);
int32_t hx711_read_average(int times);
void hx711_tare(int samples);
float hx711_get_units(int samples, float calibration_factor);

#endif