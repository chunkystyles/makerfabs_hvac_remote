#include "hvac.h"
#include "mqtt_client.h"
#include "ui.h"
#include "screen_manager.h"
#include <string.h>

using std::string;

bool isBoost = false;
bool isHorz = false;
bool isVert = false;
int32_t setPoint = 68;
char mode[32] = "Off";
bool isDoorOpen = false;

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
    doc["temperature"] = setPoint;
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
    if (setPoint != doc["temperature"]){
        setPoint = doc["temperature"];
        lv_slider_set_value(ui_Slider2, setPoint, LV_ANIM_ON);
        lv_event_send(ui_Slider2, LV_EVENT_VALUE_CHANGED, NULL);
    }
    reset_screen_timer();
}

void updateTemperatureFromMqtt(char * temperature){
    string converted = temperature;
    string newString = "Current ";
    for (int i = 0; i < sizeof(temperature); i++){
        if (isdigit(temperature[i]) || temperature[i] == '.'){
            newString += temperature[i];
        } else {
            break;
        }
    }
    newString += "°";
    lv_label_set_text(ui_Label7, newString.c_str());
}

void updateDoorFromMqtt(char * message){
    if (strncmp("open", message, 4) == 0){
        Serial.println("door open");
        isDoorOpen = true;
        lv_scr_load(ui_Screen3);
        setDoScreenDimming(false);
        reset_screen_timer();
    } else {
        Serial.println("door closed");
        if (isDoorOpen){
            Serial.println("door closed 2");
            isDoorOpen = false;
            lv_scr_load(ui_Screen1);
            setDoScreenDimming(true);
            reset_screen_timer();
        }
    }
}