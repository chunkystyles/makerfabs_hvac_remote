#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"
#include "mqtt_settings.h"

void wifi_init();

void mqtt_init();

void mqtt_loop();

void callback(char *topic, byte *payload, unsigned int length);

void connect_wifi();

void reconnect_wifi();

bool connect_mqtt(bool isReconnect);

void reconnect_mqtt();

void mqtt_publish(char *publishMessage);

void update_state(char *message);

void reboot();

void update_signal();