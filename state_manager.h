#include "mqtt_settings.h"
#include <ArduinoJson.h>

void updateBoostFromUi(bool isOn);

void updateHorzFromUi(bool isOn);

void updateVertFromUi(bool isOn);

void updateModeFromUi(char *message);

void updateSetPointFromUi(int32_t value);

void publishUpdate();

void updateStateFromMqtt(char *message);

void updateTemperatureFromMqtt(char *temperature);

void updateDoorFromMqtt(char *message);

char *get_target_label_text(int32_t value);

void mode_change_ui_update();