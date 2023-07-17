#include "state_manager.h"
#include "mqtt_client.h"
#include "ui.h"
#include "display_manager.h"
#include <string.h>

using std::string;

#define STRING_LENGTH 32
#define MAX_FAN_SPEED 11
#define MIN_FAN_SPEED 1

bool isBoost = false;
bool isHorz = false;
bool isVert = false;
int32_t setPoint = 68;
char mode[STRING_LENGTH] = "Off";
int32_t maxCool = 80;
int32_t minCool = 68;
int32_t maxHeat = 78;
int32_t minHeat = 65;
int32_t fanSpeed = 11;
char targetLabel[STRING_LENGTH];

void updateBoostFromUi(bool isOn)
{
    isBoost = isOn;
    publishUpdate();
}

void updateHorzFromUi(bool isOn)
{
    isHorz = isOn;
    publishUpdate();
}

void updateVertFromUi(bool isOn)
{
    isVert = isOn;
    publishUpdate();
}

void updateModeFromUi(char *message)
{
    strcpy(mode, message);
    publishUpdate();
    mode_change_ui_update();
}

void updateSetPointFromUi(int32_t value)
{
    if (strcmp(mode, "Fan") == 0)
    {
        fanSpeed = value;
    }
    else
    {
        setPoint = value;
    }
    publishUpdate();
}

void publishUpdate()
{
    StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
    doc["boost"] = isBoost;
    doc["horizontal"] = isHorz;
    doc["vertical"] = isVert;
    doc["mode"] = mode;
    doc["temperature"] = setPoint;
    doc["fanSpeed"] = fanSpeed;
    char output[MQTT_BUFFER_LENGTH];
    serializeJson(doc, output, MQTT_BUFFER_LENGTH);
    mqtt_publish(output);
}

void updateStateFromMqtt(char *message)
{
    bool isWakeUpdate = false;
    bool isStealthUpdate = false;
    StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    if (isBoost != doc["boost"])
    {
        isBoost = doc["boost"];
        isWakeUpdate = true;
    }
    if (isHorz != doc["horizontal"])
    {
        isHorz = doc["horizontal"];
        isWakeUpdate = true;
    }
    if (isVert != doc["vertical"])
    {
        isVert = doc["vertical"];
        isWakeUpdate = true;
    }
    if (minCool != doc["minCool"])
    {
        minCool = doc["minCool"];
        isStealthUpdate = true;
    }
    if (maxCool != doc["maxCool"])
    {
        maxCool = doc["maxCool"];
        isStealthUpdate = true;
    }
    if (minHeat != doc["minHeat"])
    {
        minHeat = doc["minHeat"];
        isStealthUpdate = true;
    }
    if (maxHeat != doc["maxHeat"])
    {
        maxHeat = doc["maxHeat"];
        isStealthUpdate = true;
    }
    if (setPoint != doc["temperature"])
    {
        setPoint = doc["temperature"];
        isWakeUpdate = true;
    }
    if (fanSpeed != doc["fanSpeed"])
    {
        fanSpeed = doc["fanSpeed"];
        isWakeUpdate = true;
    }
    if (strcmp(mode, doc["mode"]) != 0)
    {
        strcpy(mode, doc["mode"]);
        int32_t optionIndex = lv_dropdown_get_option_index(ui_Dropdown2, mode);
        if (optionIndex > -1)
        {
            lv_dropdown_set_selected(ui_Dropdown2, optionIndex);
            isWakeUpdate = true;
        }
    }
    if (isWakeUpdate || isStealthUpdate)
    {
        mode_change_ui_update();
    }
    if (isWakeUpdate)
    {
        reset_screen_timer();
    }
}

void updateTemperatureFromMqtt(char *temperature)
{
    string converted = temperature;
    string newString = "Current ";
    for (int i = 0; i < sizeof(temperature); i++)
    {
        if (isdigit(temperature[i]) || temperature[i] == '.')
        {
            newString += temperature[i];
        }
        else
        {
            break;
        }
    }
    newString += "°";
    lv_label_set_text(ui_Label7, newString.c_str());
}

void updateDoorFromMqtt(char *message)
{
    if (strncmp("open", message, 4) == 0)
    {
        lv_scr_load(ui_Screen3);
        setDoScreenDimming(false);
        reset_screen_timer();
    }
}

char *get_target_label_text(int32_t value)
{
    if (strcmp(mode, "Fan") == 0)
    {
        if (value == 11)
        {
            sprintf(targetLabel, "Fan Auto");
        }
        else
        {
            sprintf(targetLabel, "Fan speed %d", value);
        }
    }
    else if (strcmp(mode, "Off") == 0)
    {
        sprintf(targetLabel, "Off");
    }
    else
    {
        sprintf(targetLabel, "Target %d°", value);
    }
    return targetLabel;
}

void mode_change_ui_update()
{
    if (strcmp(mode, "Off") == 0)
    {
        lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Switch1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Switch2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Switch3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Label1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Label2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Label3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Label4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        if (strcmp(mode, "Fan") == 0)
        {
            lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Switch1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Switch2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Switch3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Label1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Label2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Label3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Label4, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_HIDDEN);
            lv_slider_set_range(ui_Slider2, MIN_FAN_SPEED, MAX_FAN_SPEED);
            lv_slider_set_value(ui_Slider2, fanSpeed, LV_ANIM_OFF);
            lv_event_send(ui_Slider2, LV_EVENT_VALUE_CHANGED, NULL);
        }
        else
        {
            lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Switch1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Switch2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Switch3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Label1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Label2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Label3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Label4, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Image4, LV_OBJ_FLAG_HIDDEN);
            if (strcmp(mode, "Heat") == 0)
            {
                lv_slider_set_range(ui_Slider2, minHeat, maxHeat);
            }
            else
            {
                lv_slider_set_range(ui_Slider2, minCool, maxCool);
            }
            lv_slider_set_value(ui_Slider2, setPoint, LV_ANIM_OFF);
            lv_event_send(ui_Slider2, LV_EVENT_VALUE_CHANGED, NULL);
            if (isBoost)
            {
                lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
            }
            if (isHorz)
            {
                lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
            }
            if (isVert)
            {
                lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
            }
        }
    }
}