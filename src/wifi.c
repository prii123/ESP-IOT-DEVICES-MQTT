
#include "driver/gpio.h"
#include "wifi.h"
#include "config.h"

static EventGroupHandle_t wifi_event_group;
static const int WIFI_CONNECTED_BIT = BIT0;
static const int WIFI_FAIL_BIT = BIT1;

static const char *TAG = "wifi_module";
static int retry_count = 0;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {

    gpio_set_direction(STATUS_WIFI_OK, GPIO_MODE_OUTPUT);
    gpio_set_direction(STATUS_WIFI_ERROR, GPIO_MODE_OUTPUT);
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // if (retry_count < WIFI_MAX_RETRY) {
            gpio_set_level(STATUS_WIFI_ERROR, 1); //STATUS_WIFI_OK
            gpio_set_level(STATUS_WIFI_OK, 0); //STATUS_WIFI_OK
            esp_wifi_connect();
            retry_count++;
            ESP_LOGI(TAG, "Reintentando conexión...");
        // } else {
        //     xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
        // }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        gpio_set_level(STATUS_WIFI_ERROR, 0);
        gpio_set_level(STATUS_WIFI_OK, 1);

        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Conectado! Dirección IP: " IPSTR, IP2STR(&event->ip_info.ip));
        retry_count = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_task() {
    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {

        ESP_LOGI(TAG, "Conexión establecida con %s", WIFI_SSID);
    } else if (bits & WIFI_FAIL_BIT) {

        ESP_LOGI(TAG, "No se pudo conectar a %s", WIFI_SSID);
    }

    ESP_LOGI(TAG, "Reintentos %i", WIFI_MAX_RETRY);
    vTaskDelete(NULL);  // Eliminar la tarea después de la conexión

}


void wifi_init() {
    xTaskCreate(&wifi_task, "wifi_task", 4096, NULL, 5, NULL);
}