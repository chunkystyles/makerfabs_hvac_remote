#include "hvac.h"
#include "mqtt_client.h"
#include "ui.h"

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
    if (isBoost != doc["boost"]){
        isBoost = doc["boost"];
        if (isBoost){
            lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
        }
    }
    if (isHorz != doc["horizontal"]){
        isHorz = doc["horizontal"];
        if (isHorz){
            lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
        }
    }
    if (isVert != doc["vertical"]){
        isVert = doc["vertical"];
        if (isVert){
            lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
        }
    }
    if (strcmp(mode, doc["mode"]) != 0){
        strcpy(mode, doc["mode"]);
        if (strcmp(mode, "Off") == 0){
            lv_dropdown_set_selected(ui_Dropdown2, 0);
        } else if (strcmp(mode, "Heat") == 0){
            lv_dropdown_set_selected(ui_Dropdown2, 1);
        } else if (strcmp(mode, "Cool") == 0){
            lv_dropdown_set_selected(ui_Dropdown2, 2);
        } else if (strcmp(mode, "Fan") == 0){
            lv_dropdown_set_selected(ui_Dropdown2, 3);
        }
    }
    if (setPoint != doc["setPoint"]){
        setPoint = doc["setPoint"];
        lv_slider_set_value(ui_Slider2, setPoint, LV_ANIM_ON);
        lv_event_send(ui_Slider2, LV_EVENT_VALUE_CHANGED, NULL);
    }
}