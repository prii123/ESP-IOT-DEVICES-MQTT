#ifndef ACTUADORES_H
#define ACTUADORES_H

#include "driver/gpio.h"
#include <string.h>
#include <stdio.h>
#include "config.h"


typedef enum {
    CMD_ACT_1,
    CMD_ACT_2,
    CMD_ACT_3,
    CMD_ACT_4,
    CMD_UNKNOWN
} CommandType;


// Configuración de GPIOs
void setup_gpio();

// Conversión de string a enum
CommandType get_command(const char *data, int data_len);

// Manejo de mensajes MQTT
void handle_mqtt_message(const char *topic, int topic_len, const char *data, int data_len);

#endif // ACTUADORES_H
