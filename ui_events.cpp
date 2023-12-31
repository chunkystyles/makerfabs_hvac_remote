// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.3
// Project name: 7_5_23

#include "ui.h"
#include "state_manager.h"
#include "display_manager.h"
#include <Arduino.h>

void screen_clicked(lv_event_t *e)
{
	reset_screen_timer();
}

void door_screen_clicked(lv_event_t *e)
{
	lv_scr_load(ui_main_screen);
	set_do_screen_dimming(true);
	reset_screen_timer();
}

void boost_slider_checked(lv_event_t *e)
{
	update_boost(true);
	reset_screen_timer();
}

void boost_slider_unchecked(lv_event_t *e)
{
	update_boost(false);
	reset_screen_timer();
}

void mode_changed(lv_event_t *e)
{
	char buf[32];
	lv_dropdown_get_selected_str(e->target, buf, sizeof(buf));
	update_mode(buf);
	reset_screen_timer();
}

void slider_released(lv_event_t *e)
{
	int32_t value = lv_slider_get_value(e->target);
	update_target_temperature(value);
	reset_screen_timer();
}

void swing_vert_slider_checked(lv_event_t *e)
{
	update_vertical(true);
	reset_screen_timer();
}

void swing_vert_slider_unchecked(lv_event_t *e)
{
	update_vertical(false);
	reset_screen_timer();
}

void swing_horz_checked(lv_event_t *e)
{
	update_horizontal(true);
	reset_screen_timer();
}

void swing_horz_unchecked(lv_event_t *e)
{
	update_horizontal(false);
	reset_screen_timer();
}

char *get_slider_label_text(lv_obj_t *target)
{
	return get_target_label_text(lv_slider_get_value(target));
}
