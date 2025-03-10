
#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define WIFI_SSID      "SOMOSBRAYAN-2.4G"
#define WIFI_PASS      "04373847Vallejos"
#define WIFI_MAX_RETRY 5

#define CONFIG_BROKER_URL "mqtt://192.168.78.107"

#define RELAY_GPIO  35

/**
 * definicion de estados por medio de leds
 *  STATUS_WIFI_OK
 *  STATUS_WIFI_ERROR
 *  STATUS_MQTT_OK
 *  STATUS_MQTT_ERROR
 */

 #define STATUS_WIFI_OK 2
 #define STATUS_WIFI_ERROR 25
 #define STATUS_MQTT_OK 33
 #define STATUS_MQTT_ERROR 26
 
 /**
 * TOPIC
 * HP  HABITACION PRINCIPAL
 * IL  ILUMINACION
 */
//SUSCRIPCIONES
#define TOPIC_HP_IL_ALL "TOPIC_HP_IL_ALL"
#define TOPIC_HP_IL_1 "TOPIC_HP_IL_1"
#define TOPIC_HP_IL_2 "TOPIC_HP_IL_2"
#define TOPIC_HP_IL_3 "TOPIC_HP_IL_3"
#define TOPIC_HP_IL_4 "TOPIC_HP_IL_4"
//PUBLICACIONES
#define TOPIC_HP_IL_1_STATUS "TOPIC_HP_IL_1_STATUS"
#define TOPIC_HP_IL_2_STATUS "TOPIC_HP_IL_2_STATUS"
#define TOPIC_HP_IL_3_STATUS "TOPIC_HP_IL_3_STATUS"
#define TOPIC_HP_IL_4_STATUS "TOPIC_HP_IL_4_STATUS"
//ESTADOS
extern bool act_1_status;
extern bool act_2_status;
extern bool act_3_status;
extern bool act_4_status;
//PINES A UTILIZAR
#define GPIO_ACT_1 GPIO_NUM_25
#define GPIO_ACT_2 GPIO_NUM_21
#define GPIO_ACT_3 GPIO_NUM_27
#define GPIO_ACT_4 GPIO_NUM_14





#endif // CONFIG_H
