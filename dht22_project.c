#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include "dht22.h"
#include "webserver.h"
#include "sensor_data.h"
#include "wifi.h"

static const char *TAG = "APP";

/* GLOBAL SENSOR VALUES */
float g_temp = 0;
float g_hum  = 0;

/* =====================================================
   SENSOR TASK
   ===================================================== */
void sensor_task(void *pvParameters)
{
    while (1)
    {
        float t = 0;
        float h = 0;
        esp_err_t ret;

        // 🔥 retry up to 3 times
        for (int i = 0; i < 3; i++)
        {
            ret = dht22_read(&t, &h);
            if (ret == ESP_OK) break;
            vTaskDelay(pdMS_TO_TICKS(50)); // small recovery delay
        }

        if (ret == ESP_OK)
        {
            g_temp = t;
            g_hum  = h;

            ESP_LOGI("APP",
                "Temp: %.1f°C | Humidity: %.1f%%",
                g_temp, g_hum);
        }
        else
        {
            ESP_LOGE("APP", "DHT22 failed after retries");
        }

        vTaskDelay(pdMS_TO_TICKS(3000)); // keep stable interval
    }
}

/* =====================================================
   MAIN
   ===================================================== */
void app_main(void)
{
    ESP_LOGI(TAG, "System starting...");

    /* NVS REQUIRED FOR WIFI */
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    /* INIT SENSOR */
    dht22_init();

    /* START WIFI */
    wifi_init_sta();

    /* START WEB SERVER */
    start_webserver();

    /* SENSOR TASK */
    xTaskCreatePinnedToCore(
        sensor_task,
        "sensor_task",
        4096,
        NULL,
        5,
        NULL,
        1);
}
