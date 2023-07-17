#include "state_manager.h"
#include "mqtt_client.h"
#include "ui.h"
#include "display_manager.h"
#include <string.h>

using std::string;

#define STRING_LENGTH 32
#define MAX_FAN_SPEED 11
#define MIN_FAN_SPEED 1

bool is_boost = false;
bool is_horizontal = false;
bool is_vertical = false;
int32_t target_temperature = 68;
char mode[STRING_LENGTH] = "Off";
int32_t max_cool = 80;
int32_t min_cool = 68;
int32_t max_heat = 78;
int32_t min_heat = 65;
int32_t fan_speed = 11;
char target_label_text[STRING_LENGTH];

void update_boost(bool is_on)
{
    is_boost = is_on;
    publish_update();
}

void update_horizontal(bool is_on)
{
    is_horizontal = is_on;
    publish_update();
}

void update_vertical(bool is_on)
{
    is_vertical = is_on;
    publish_update();
}

void update_mode(char *message)
{
    strcpy(mode, message);
    publish_update();
    update_ui();
}

void update_target_temperature(int32_t value)
{
    if (strcmp(mode, "Fan") == 0)
    {
        fan_speed = value;
    }
    else
    {
        target_temperature = value;
    }
    publish_update();
}

void publish_update()
{
    StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
    doc["boost"] = is_boost;
    doc["horizontal"] = is_horizontal;
    doc["vertical"] = is_vertical;
    doc["mode"] = mode;
    doc["temperature"] = target_temperature;
    doc["fanSpeed"] = fan_speed;
    char output[MQTT_BUFFER_LENGTH];
    serializeJson(doc, output, MQTT_BUFFER_LENGTH);
    mqtt_publish(output);
}

void update_state(char *message)
{
    bool is_wake_update = false;
    bool is_stealth_update = false;
    StaticJsonDocument<MQTT_BUFFER_LENGTH> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    if (is_boost != doc["boost"])
    {
        is_boost = doc["boost"];
        is_wake_update = true;
    }
    if (is_horizontal != doc["horizontal"])
    {
        is_horizontal = doc["horizontal"];
        is_wake_update = true;
    }
    if (is_vertical != doc["vertical"])
    {
        is_vertical = doc["vertical"];
        is_wake_update = true;
    }
    if (min_cool != doc["minCool"])
    {
        min_cool = doc["minCool"];
        is_stealth_update = true;
    }
    if (max_cool != doc["maxCool"])
    {
        max_cool = doc["maxCool"];
        is_stealth_update = true;
    }
    if (min_heat != doc["minHeat"])
    {
        min_heat = doc["minHeat"];
        is_stealth_update = true;
    }
    if (max_heat != doc["maxHeat"])
    {
        max_heat = doc["maxHeat"];
        is_stealth_update = true;
    }
    if (target_temperature != doc["temperature"])
    {
        target_temperature = doc["temperature"];
        is_wake_update = true;
    }
    if (fan_speed != doc["fanSpeed"])
    {
        fan_speed = doc["fanSpeed"];
        is_wake_update = true;
    }
    if (strcmp(mode, doc["mode"]) != 0)
    {
        strcpy(mode, doc["mode"]);
        int32_t option_index = lv_dropdown_get_option_index(ui_Dropdown2, mode);
        if (option_index > -1)
        {
            lv_dropdown_set_selected(ui_Dropdown2, option_index);
            is_wake_update = true;
        }
    }
    if (is_wake_update || is_stealth_update)
    {
        update_ui();
    }
    if (is_wake_update)
    {
        reset_screen_timer();
    }
}

void update_temperature(char *temperature)
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

void update_door(char *message)
{
    if (strncmp("open", message, 4) == 0)
    {
        lv_scr_load(ui_Screen3);
        set_do_screen_dimming(false);
        reset_screen_timer();
    }
}

char *get_target_label_text(int32_t value)
{
    if (strcmp(mode, "Fan") == 0)
    {
        if (value == 11)
        {
            sprintf(target_label_text, "Fan Auto");
        }
        else
        {
            sprintf(target_label_text, "Fan speed %d", value);
        }
    }
    else if (strcmp(mode, "Off") == 0)
    {
        sprintf(target_label_text, "Off");
    }
    else
    {
        sprintf(target_label_text, "Target %d°", value);
    }
    return target_label_text;
}

void update_ui()
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
            lv_slider_set_value(ui_Slider2, fan_speed, LV_ANIM_OFF);
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
                lv_slider_set_range(ui_Slider2, min_heat, max_heat);
            }
            else
            {
                lv_slider_set_range(ui_Slider2, min_cool, max_cool);
            }
            lv_slider_set_value(ui_Slider2, target_temperature, LV_ANIM_OFF);
            lv_event_send(ui_Slider2, LV_EVENT_VALUE_CHANGED, NULL);
            if (is_boost)
            {
                lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
            }
            if (is_horizontal)
            {
                lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
            }
            if (is_vertical)
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