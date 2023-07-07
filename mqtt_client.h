#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"
#include "mqtt_settings.h"

void wifi_init();

void mqtt_init();

void mqtt_loop();

void callback(char* topic, byte* payload, unsigned int length);

void connect(bool isReconnect);

void mqtt_publish(char * publishMessage);

void updateStateFromMqtt(char * message);