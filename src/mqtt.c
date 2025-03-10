
#include "config.h"
#include "actuadores.h"
#include <stdint.h>
#include "esp_err.h"
#include "esp_event.h"

#include "mqtt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include <string.h>



static const char *TAG = "mqtt";
static esp_mqtt_client_handle_t client;



static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    gpio_set_direction(STATUS_MQTT_ERROR, GPIO_MODE_OUTPUT);
    gpio_set_direction(STATUS_MQTT_OK, GPIO_MODE_OUTPUT);

    esp_mqtt_event_handle_t event = event_data;
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            gpio_set_level(STATUS_MQTT_OK, 1);
            gpio_set_level(STATUS_MQTT_ERROR, 0);
            ESP_LOGI(TAG, "Conectado a MQTT.");
            esp_mqtt_client_subscribe(client, TOPIC_HP_IL_ALL, 0);
            esp_mqtt_client_subscribe(client, TOPIC_HP_IL_1, 0);
            esp_mqtt_client_subscribe(client, TOPIC_HP_IL_2, 0);
            esp_mqtt_client_subscribe(client, TOPIC_HP_IL_3, 0);
            esp_mqtt_client_subscribe(client, TOPIC_HP_IL_4, 0);

            break;
        case MQTT_EVENT_DISCONNECTED:
            gpio_set_level(STATUS_MQTT_OK, 0);
            gpio_set_level(STATUS_MQTT_ERROR, 1);
            ESP_LOGI(TAG, "Desconectado de MQTT.");
            break;
        case MQTT_EVENT_DATA:
            // ESP_LOGI(TAG, "Mensaje recibido en %.*s: %.*s",
            //     event->topic_len, event->topic,
            //     event->data_len, event->data);
            // Reservar memoria dinámicamente para topic y data
            char *topic = malloc(event->topic_len + 1);
            char *data = malloc(event->data_len + 1);

            if (topic && data) {
                strncpy(topic, event->topic, event->topic_len);
                topic[event->topic_len] = '\0';

                strncpy(data, event->data, event->data_len);
                data[event->data_len] = '\0';

                // Llamar a la función para manejar el mensaje
                handle_mqtt_message(topic, event->topic_len, data, event->data_len);
            } else {
                ESP_LOGE(TAG, "Error de asignación de memoria");
            }

            // Liberar memoria después de usar
            free(topic);
            free(data);
            break;

        default:
            break;
    }
}

void mqtt_task(void *pvParameter) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL
    };


    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Mantener la tarea activa
    }
}

void mqtt_start(void) {
    xTaskCreate(&mqtt_task, "mqtt_task", 4096, NULL, 5, NULL);
}

void mqtt_publish(const char *topic, const char *message) {
    esp_mqtt_client_publish(client, topic, message, 0, 0, 0);
}

/**
 * publicacion a los topics de estados  sensores 
 * se agregan a tareas 
 */
void publicar_rapido(void *pvParameters) {
    while (1) {
        mqtt_publish(TOPIC_HP_IL_1_STATUS, (act_1_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(3000));

        mqtt_publish(TOPIC_HP_IL_2_STATUS, (act_2_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(3000));

        mqtt_publish(TOPIC_HP_IL_3_STATUS, (act_3_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(3000));

        mqtt_publish(TOPIC_HP_IL_4_STATUS, (act_4_status ? "true" : "false"));
        vTaskDelay(pdMS_TO_TICKS(3000));

    }
}

void publicar_lento(void *pvParameters) {
    while (1) {
        mqtt_publish("topic/lento", "Dato cada 5s");
        vTaskDelay(pdMS_TO_TICKS(5000)); // Espera 5 segundos
    }
}

void publicacionTask(void) {
    xTaskCreate(&publicar_rapido, "Publicar_Rapido", 4096, NULL, 5, NULL);
    xTaskCreate(&publicar_lento, "Publicar_Lento", 4096, NULL, 5, NULL);
}

