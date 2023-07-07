// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.3
// Project name: 7_5_23

#include "ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Slider2 = lv_slider_create(ui_Screen1);
    lv_slider_set_range(ui_Slider2, 68, 80);
    lv_obj_set_width(ui_Slider2, 50);
    lv_obj_set_height(ui_Slider2, 225);
    lv_obj_set_x(ui_Slider2, 189);
    lv_obj_set_y(ui_Slider2, 0);
    lv_obj_set_align(ui_Slider2, LV_ALIGN_CENTER);

    ui_Label2 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, 49);
    lv_obj_set_y(ui_Label2, -109);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "Set Point °");
    lv_obj_set_style_text_font(ui_Label2, &ui_font_MMBold24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch1 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch1, 100);
    lv_obj_set_height(ui_Switch1, 50);
    lv_obj_set_x(ui_Switch1, -160);
    lv_obj_set_y(ui_Switch1, -30);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    ui_Label4 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 2
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label4, -65);
    lv_obj_set_y(ui_Label4, -30);
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "Boost");
    lv_obj_set_style_text_font(ui_Label4, &ui_font_MMBold24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Dropdown2 = lv_dropdown_create(ui_Screen1);
    lv_dropdown_set_options(ui_Dropdown2, "Off\nHeat\nCool\nFan");
    lv_obj_set_width(ui_Dropdown2, 103);
    lv_obj_set_height(ui_Dropdown2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Dropdown2, -155);
    lv_obj_set_y(ui_Dropdown2, -110);
    lv_obj_set_align(ui_Dropdown2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Dropdown2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_text_font(ui_Dropdown2, &ui_font_MMBold24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(ui_Dropdown2, &lv_font_montserrat_14, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(lv_dropdown_get_list(ui_Dropdown2), &ui_font_MMBold24,  LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch2 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch2, 100);
    lv_obj_set_height(ui_Switch2, 50);
    lv_obj_set_x(ui_Switch2, -160);
    lv_obj_set_y(ui_Switch2, 40);
    lv_obj_set_align(ui_Switch2, LV_ALIGN_CENTER);

    ui_Switch3 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch3, 100);
    lv_obj_set_height(ui_Switch3, 50);
    lv_obj_set_x(ui_Switch3, -160);
    lv_obj_set_y(ui_Switch3, 110);
    lv_obj_set_align(ui_Switch3, LV_ALIGN_CENTER);

    ui_Label1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 2
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1, -65);
    lv_obj_set_y(ui_Label1, 40);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "Swing");
    lv_obj_set_style_text_font(ui_Label1, &ui_font_MMBold24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label3 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 2
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, -65);
    lv_obj_set_y(ui_Label3, 110);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "Swing");
    lv_obj_set_style_text_font(ui_Label3, &ui_font_MMBold24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image1 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image1, &ui_img_bw_arrow_png);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image1, -15);
    lv_obj_set_y(ui_Image1, 43);
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_angle(ui_Image1, 1800);
    lv_img_set_zoom(ui_Image1, 100);

    ui_Image2 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image2, &ui_img_bw_arrow_png);
    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image2, 9);
    lv_obj_set_y(ui_Image2, 43);
    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image2, 100);

    ui_Image3 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image3, &ui_img_bw_arrow_png);
    lv_obj_set_width(ui_Image3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image3, 11);
    lv_obj_set_y(ui_Image3, 111);
    lv_obj_set_align(ui_Image3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_angle(ui_Image3, 2700);
    lv_img_set_zoom(ui_Image3, 100);

    ui_Image4 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image4, &ui_img_bw_arrow_png);
    lv_obj_set_width(ui_Image4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image4, -15);
    lv_obj_set_y(ui_Image4, 111);
    lv_obj_set_align(ui_Image4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_angle(ui_Image4, 900);
    lv_img_set_zoom(ui_Image4, 100);

    lv_obj_add_event_cb(ui_Slider2, ui_event_Slider2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Switch1, ui_event_Switch1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Dropdown2, ui_event_Dropdown2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Switch2, ui_event_Switch2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Switch3, ui_event_Switch3, LV_EVENT_ALL, NULL);

}
