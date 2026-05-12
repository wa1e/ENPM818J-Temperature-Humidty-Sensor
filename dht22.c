#include "dht22.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DHT_GPIO GPIO_NUM_18

static const char *TAG = "DHT22";

static int wait_for_level(int level, int timeout_us)
{
    int count = 0;

    while (gpio_get_level(DHT_GPIO) == level) {
        esp_rom_delay_us(1);
        if (++count > timeout_us) return -1;
    }
    return count;
}

void dht22_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << DHT_GPIO),
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };

    gpio_config(&io_conf);
    gpio_set_level(DHT_GPIO, 1);
}

esp_err_t dht22_read(float *temperature, float *humidity)
{
    uint8_t data[5] = {0};

    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT_OD);
    gpio_set_level(DHT_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(20));

    gpio_set_level(DHT_GPIO, 1);
    gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);

    esp_rom_delay_us(40);

    if (wait_for_level(1, 5000) < 0) return ESP_FAIL;
    if (wait_for_level(0, 5000) < 0) return ESP_FAIL;
    if (wait_for_level(1, 5000) < 0) return ESP_FAIL;

    for (int i = 0; i < 40; i++) {

        if (wait_for_level(0, 1000) < 0) return ESP_FAIL;

        int high = wait_for_level(1, 2000);
        if (high < 0) return ESP_FAIL;

        data[i / 8] <<= 1;

        if (high > 40)
            data[i / 8] |= 1;
    }

    uint8_t sum = data[0] + data[1] + data[2] + data[3];
    if (sum != data[4]) return ESP_FAIL;

    uint16_t rh = (data[0] << 8) | data[1];
    uint16_t rt = ((data[2] & 0x7F) << 8) | data[3];

    *humidity = rh / 10.0f;
    *temperature = rt / 10.0f;

    if (data[2] & 0x80)
        *temperature *= -1;

    return ESP_OK;
}
