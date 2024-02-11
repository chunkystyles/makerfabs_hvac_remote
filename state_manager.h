#include "mqtt_settings.h"
#include <ArduinoJson.h>

void update_boost(bool isOn);

void update_horizontal(bool isOn);

void update_vertical(bool isOn);

void update_mode(char *message);

void update_target_temperature(int32_t value);

void publish_update();

void update_state(char *message);

void update_temperature(char *temperature, unsigned int length);

void update_thermostat(char *temperature, unsigned int length);

void update_door(char *message);

char *get_target_label_text(int32_t value);

void update_ui();