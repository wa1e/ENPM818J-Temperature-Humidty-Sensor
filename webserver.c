#include "webserver.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "sensor_data.h"

static const char *TAG = "WEB";

/* =====================================================
   HTTP HANDLER
   ===================================================== */
static esp_err_t root_handler(httpd_req_t *req)
{
    char html[400];

    snprintf(html, sizeof(html),
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta http-equiv='refresh' content='2'>"
        "<title>DHT22 Monitor</title>"
        "</head>"

        "<body style='font-family:Arial;text-align:center;'>"

        "<h1>DHT22 Live Data</h1>"

        "<h2>Temperature: %.2f &deg;C</h2>"

        "<h2>Humidity: %.2f %%</h2>"

        "</body>"
        "</html>",

        g_temp,
        g_hum);

    httpd_resp_set_type(req, "text/html");

    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

/* =====================================================
   START SERVER
   ===================================================== */
void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_handler
        };

        httpd_register_uri_handler(server, &uri);

        ESP_LOGI(TAG, "Web server started");
    }
}
