
#include "config.h"
#include <stdio.h>
#include "esp_err.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "mqtt.h"
#include "wifi.h"


static const char *TAG = "app_main";


void app_main(void) {
    wifi_init();  // Iniciar WiFi
    mqtt_start(); // Iniciar MQTT
    
    //LOOP PARA PUBLICAR EL ESTADO DE LOS DISPOSITIVOS
    while (1) {
        // ESP_LOGI(TAG, "Publicando mensaje...");
        // ESP_LOGI(TAG, "Valor de act_1_status: %s", act_1_status ? "true" : "false");
        mqtt_publish(TOPIC_HP_IL_1_STATUS, (act_1_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(1000));

        mqtt_publish(TOPIC_HP_IL_2_STATUS, (act_2_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(1000));

        mqtt_publish(TOPIC_HP_IL_3_STATUS, (act_3_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(1000));

        mqtt_publish(TOPIC_HP_IL_4_STATUS, (act_4_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(1000));

    }

}

