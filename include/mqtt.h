


#ifndef MQTT_H
#define MQTT_H

#include "freertos/queue.h"

void mqtt_start(void);
void mqtt_publish(const char *topic, const char *message);
void publicacionTask(void);


#endif // MQTT_H



