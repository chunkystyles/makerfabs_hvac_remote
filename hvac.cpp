#include "hvac.h"
#include "mqtt_client.h"

bool isBoost = false;
bool isHorz = false;
bool isVert = false;
int32_t setPoint = 68;
char mode[32] = "Off";

void updateBoostFromUi(bool isOn){
    isBoost = isOn;
    publishUpdate();
}

void updateHorzFromUi(bool isOn){
    isHorz = isOn;
    publishUpdate();
}

void updateVertFromUi(bool isOn){
    isVert = isOn;
    publishUpdate();
}

void updateModeFromUi(char * message){
    strcpy(mode, message);
    publishUpdate();
}

void updateSetPointFromUi(int32_t value){
    setPoint = value;
    publishUpdate();
}

void publishUpdate(){
    StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
    doc["boost"] = isBoost;
    doc["horizontal"] = isHorz;
    doc["vertical"] = isVert;
    doc["mode"] = mode;
    doc["setPoint"] = setPoint;
    char output[MQTT_BUFFER_LENGTH];
    serializeJson(doc, output, MQTT_BUFFER_LENGTH);
    mqtt_publish(output);
}

void updateStateFromMqtt(char * message){
    StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    isBoost = doc["boost"];
    isHorz = doc["horizontal"];
    isVert = doc["vertical"];
    strcpy(mode, doc["mode"]);
    setPoint = doc["setPoint"];
}