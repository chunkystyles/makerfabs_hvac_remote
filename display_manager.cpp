#include "display_manager.h"

LGFX lcd;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[MY_SCREEN_WIDTH * 10];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_timer_t *timer;
static uint32_t user_data = 10;
const uint16_t timeout = 5000;
bool is_screen_dimmed = false;
bool is_screen_off = false;
bool do_screen_dimming = true;

void lcd_init()
{
    // Pin init
    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_BLK, OUTPUT);
    digitalWrite(LCD_CS, LOW);
    digitalWrite(LCD_BLK, HIGH);

    // lcd init
    lcd.init();
    lcd.setRotation(MY_SCREEN_ROTATION);
    lcd.fillScreen(COLOR_BACKGROUND);
    lcd.setTextColor(COLOR_TEXT);
    lcd.setBrightness(MY_MAX_BRIGHTNESS);

    // I2C init
    Wire.begin(I2C_SDA, I2C_SCL);
    byte error, address;

    Wire.beginTransmission(TOUCH_I2C_ADD);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.print("I2C device found at address 0x");
        Serial.print(TOUCH_I2C_ADD, HEX);
        Serial.println("  !");
    }
    else
    {
        Serial.print("Unknown error at address 0x");
        Serial.println(TOUCH_I2C_ADD, HEX);
    }
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    lcd.endWrite();
    lv_disp_flush_ready(disp);
}

void lvgl_init()
{
    lcd_init();
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, MY_SCREEN_WIDTH * 10);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = MY_SCREEN_WIDTH;
    disp_drv.ver_res = MY_SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    lv_disp_drv_register(&disp_drv);
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    timer = lv_timer_create(screen_timer_event, MY_SCREEN_TIMER, &user_data);
}

void screen_timer_event(lv_timer_t *timer)
{
    if (do_screen_dimming && !is_screen_off)
    {
        if (!is_screen_dimmed)
        {
            is_screen_dimmed = true;
            lcd.setBrightness(MY_DIM_BRIGHTNESS);
        }
        else
        {
            is_screen_off = true;
            lcd.setBrightness(MY_MIN_BRIGHTNESS);
            lv_scr_load(ui_Screen2);
        }
    }
}

void reset_screen_timer()
{
    lcd.setBrightness(MY_MAX_BRIGHTNESS);
    lv_timer_reset(timer);
    if (is_screen_off && ui_Screen2 == lv_scr_act())
    {
        lv_scr_load(ui_Screen1);
    }
    is_screen_dimmed = false;
    is_screen_off = false;
}

void set_do_screen_dimming(bool input)
{
    if (input != do_screen_dimming)
    {
        do_screen_dimming = input;
    }
}