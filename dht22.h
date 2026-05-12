#pragma once

#include "esp_err.h"

void dht22_init(void);
esp_err_t dht22_read(float *temperature, float *humidity);
