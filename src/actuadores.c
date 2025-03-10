#include "driver/gpio.h"
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "actuadores.h"


// typedef enum {
//     CMD_ACT_1,
//     CMD_ACT_2,
//     CMD_ACT_3,
//     CMD_ACT_4,
//     CMD_UNKNOWN
// } CommandType;

bool act_1_status = false;
bool act_2_status = false;
bool act_3_status = false;
bool act_4_status = false;


void setup_gpio() {
    gpio_reset_pin(GPIO_ACT_1);
    gpio_reset_pin(GPIO_ACT_2);
    gpio_reset_pin(GPIO_ACT_3);
    gpio_reset_pin(GPIO_ACT_4);

    gpio_set_direction(GPIO_ACT_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_ACT_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_ACT_3, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_ACT_4, GPIO_MODE_OUTPUT);  
}

// Función para convertir string a enum 
CommandType get_command(const char *data, int data_len) {
    if (strncmp(data, TOPIC_HP_IL_1, data_len) == 0) return CMD_ACT_1;
    if (strncmp(data, TOPIC_HP_IL_2, data_len) == 0) return CMD_ACT_2;
    if (strncmp(data, TOPIC_HP_IL_3, data_len) == 0) return CMD_ACT_3;
    if (strncmp(data, TOPIC_HP_IL_4, data_len) == 0) return CMD_ACT_4;
    if (strncmp(data, TOPIC_HP_IL_ALL, data_len) == 0) return CMD_ACT_5;
    return CMD_UNKNOWN;
}

void handle_mqtt_message(const char *topic, int topic_len, const char *data, int data_len) {
    CommandType command = get_command(topic, data_len);

        switch (command) {
            case CMD_ACT_1:
                act_1_status = !act_1_status;
                gpio_set_level(GPIO_ACT_1, act_1_status);
                printf("ACT_1 cambiado a: %d\n", act_1_status);
                break;
    
            case CMD_ACT_2:
                act_2_status = !act_2_status;
                gpio_set_level(GPIO_ACT_2, act_2_status);
                printf("ACT_2 cambiado a: %d\n", act_2_status);
                break;
    
            case CMD_ACT_3:
                act_3_status = !act_3_status;
                gpio_set_level(GPIO_ACT_3, act_3_status);
                printf("ACT_3 cambiado a: %d\n", act_3_status);
                break;
    
            case CMD_ACT_4:
                act_4_status = !act_4_status;
                gpio_set_level(GPIO_ACT_4, act_4_status);
                printf("ACT_4 cambiado a: %d\n", act_4_status);
                break;

            case CMD_ACT_5:
                act_1_status = !act_1_status;
                act_2_status = !act_2_status;
                act_3_status = !act_3_status;
                act_4_status = !act_4_status;
                gpio_set_level(GPIO_ACT_1, act_1_status);
                gpio_set_level(GPIO_ACT_2, act_2_status);
                gpio_set_level(GPIO_ACT_3, act_3_status);
                gpio_set_level(GPIO_ACT_4, act_4_status);
                printf("Todos los estados cambiaron a: %d\n", act_4_status);
                break;
    
            case CMD_UNKNOWN:
                printf("Comando desconocido: %.*s\n", data_len, data);
                break;
        }
}

