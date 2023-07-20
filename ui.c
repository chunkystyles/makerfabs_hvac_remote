// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.3
// Project name: 7_5_23

#include "ui.h"
#include "ui_helpers.h"
#include "ui_events.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_main_screen
void ui_main_screen_screen_init(void);
void ui_event_main_screen(lv_event_t *e);
lv_obj_t *ui_main_screen;
void ui_event_main_slider(lv_event_t *e);
lv_obj_t *ui_main_slider;
lv_obj_t *ui_target_temp_label;
void ui_event_boost_switch(lv_event_t *e);
lv_obj_t *ui_boost_switch;
lv_obj_t *ui_boost_label;
void ui_event_mode_dropdown(lv_event_t *e);
lv_obj_t *ui_mode_dropdown;
void ui_event_fan_vert_switch(lv_event_t *e);
lv_obj_t *ui_fan_vert_switch;
void ui_event_fan_horz_switch(lv_event_t *e);
lv_obj_t *ui_fan_horz_switch;
lv_obj_t *ui_fan_vert_label;
lv_obj_t *ui_fan_horz_label;
lv_obj_t *ui_arrow_up_image;
lv_obj_t *ui_arrow_down_image;
lv_obj_t *ui_arrow_right_image;
lv_obj_t *ui_arrow_left_image;
lv_obj_t *ui_mode_label;
lv_obj_t *ui_current_temp_label;

// SCREEN: ui_blank_screen
void ui_blank_screen_screen_init(void);
void ui_event_blank_screen(lv_event_t *e);
lv_obj_t *ui_blank_screen;

// SCREEN: ui_door_screen
void ui_door_screen_screen_init(void);
void ui_event_door_screen(lv_event_t *e);
lv_obj_t *ui_door_screen;
void ui_event_door_button(lv_event_t *e);
lv_obj_t *ui_door_button;
lv_obj_t *ui_door_button_label;
void ui_event_door_label(lv_event_t *e);
lv_obj_t *ui_door_label;

// SCREEN: ui_reconnect_screen
void ui_reconnect_screen_screen_init(void);
void ui_event_reconnect_screen(lv_event_t *e);
lv_obj_t *ui_reconnect_screen;
void ui_event_reconnect_label(lv_event_t *e);
lv_obj_t *ui_reconnect_label;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP != 0
#error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_main_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_RELEASED)
    {
        screen_clicked(e);
    }
}
void ui_event_main_slider(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        lv_label_set_text(ui_target_temp_label, get_slider_label_text(target));
        screen_clicked(e);
    }
    if (event_code == LV_EVENT_RELEASED)
    {
        slider_released(e);
    }
    if (event_code == LV_EVENT_CLICKED)
    {
        screen_clicked(e);
    }
}
void ui_event_boost_switch(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target, LV_STATE_CHECKED))
    {
        boost_slider_checked(e);
    }
    if (event_code == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(target, LV_STATE_CHECKED))
    {
        boost_slider_unchecked(e);
        _ui_state_modify(ui_main_screen, LV_STATE_CHECKED, _UI_MODIFY_STATE_ADD);
    }
}
void ui_event_mode_dropdown(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        mode_changed(e);
    }
    if (event_code == LV_EVENT_CLICKED)
    {
        screen_clicked(e);
    }
}
void ui_event_fan_vert_switch(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target, LV_STATE_CHECKED))
    {
        swing_vert_slider_checked(e);
    }
    if (event_code == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(target, LV_STATE_CHECKED))
    {
        swing_vert_slider_unchecked(e);
    }
}
void ui_event_fan_horz_switch(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target, LV_STATE_CHECKED))
    {
        swing_horz_checked(e);
    }
    if (event_code == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(target, LV_STATE_CHECKED))
    {
        swing_horz_unchecked(e);
    }
}
void ui_event_blank_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        screen_clicked(e);
    }
}
void ui_event_door_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_PRESSED)
    {
        door_screen_clicked(e);
    }
}
void ui_event_door_button(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_PRESSED)
    {
        door_screen_clicked(e);
    }
}
void ui_event_door_label(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        door_screen_clicked(e);
    }
}
void ui_event_reconnect_screen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        screen_clicked(e);
    }
}
void ui_event_reconnect_label(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        screen_clicked(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                              true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_main_screen_screen_init();
    ui_blank_screen_screen_init();
    ui_door_screen_screen_init();
    ui_reconnect_screen_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_main_screen);
}
