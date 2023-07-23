// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.3
// Project name: 7_5_23

#include "../ui.h"

void ui_reconnect_screen_screen_init(void)
{
    ui_reconnect_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_reconnect_screen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_reconnect_label = lv_label_create(ui_reconnect_screen);
    lv_obj_set_width(ui_reconnect_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_reconnect_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_reconnect_label, LV_ALIGN_CENTER);
    lv_label_set_text(ui_reconnect_label, "Attempting to reconnect...");
    lv_obj_add_flag(ui_reconnect_label, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_set_style_text_font(ui_reconnect_label, &ui_font_MMBold24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_reconnect_label, ui_event_reconnect_label, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_reconnect_screen, ui_event_reconnect_screen, LV_EVENT_ALL, NULL);

}
