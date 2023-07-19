#include "display_settings.h"
#include "touch_screen_driver.h"
#include "display_driver.h"
#include "secrets.h"

#include <lvgl.h>
#include "ui.h"

#define I2C_SCL 39
#define I2C_SDA 38

#define COLOR_BACKGROUND TFT_BLACK
#define COLOR_BUTTON TFT_WHITE
#define COLOR_BUTTON_P TFT_YELLOW
#define COLOR_TEXT 0x322B
#define COLOR_LINE TFT_BLACK
#define COLOR_SHADOW 0x4BAF

void lcd_init();

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void lvgl_init();

void screen_timer_event(lv_timer_t *timer);

void reset_screen_timer();

void set_do_screen_dimming(bool input);