#include "FT6236.h"
#include "Parallel16_9488.h"
#include <lvgl.h>
#include "ui.h"

#define I2C_SCL 39
#define I2C_SDA 38

#define COLOR_BACKGROUND 0xEF9E
#define COLOR_BUTTON TFT_WHITE
#define COLOR_BUTTON_P TFT_YELLOW
#define COLOR_TEXT 0x322B
#define COLOR_LINE TFT_BLACK
#define COLOR_SHADOW 0x4BAF

LGFX lcd;

// Rotation:
// 0 = none
// 1 = 90 counter clockwise
// 2 = 180
// 3 = 270 counter clockwise
static const uint_fast8_t screenRotation = 1;
static const uint32_t screenWidth = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t x, y;
  if (ft6236_coords(&x, &y)) {
    data->state = LV_INDEV_STATE_PR;
    switch(screenRotation){
      case 1:
        data->point.x = y;
        data->point.y = screenHeight - x;
        break;
      case 2:
        data->point.x = screenWidth - x;
        data->point.y = screenHeight - y;
        break;
      case 3:
        data->point.x = screenWidth - y;
        data->point.y = x;
        break;
      default:
        data->point.x = x;
        data->point.y = y;
        break;
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Setup start");
    lcd_init();
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    lv_disp_drv_register(&disp_drv);
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    ui_init();
}

void loop() {
    lv_timer_handler();
    delay(5);
}

void lcd_init() {
    // Pin init
    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_BLK, OUTPUT);
    digitalWrite(LCD_CS, LOW);
    digitalWrite(LCD_BLK, HIGH);

    // lcd init
    lcd.init();
    lcd.setRotation(screenRotation);
    lcd.fillScreen(COLOR_BACKGROUND);
    lcd.setTextColor(COLOR_TEXT);

    // I2C init
    Wire.begin(I2C_SDA, I2C_SCL);
    byte error, address;

    Wire.beginTransmission(TOUCH_I2C_ADD);
    error = Wire.endTransmission();
    if (error == 0) {
        Serial.print("I2C device found at address 0x");
        Serial.print(TOUCH_I2C_ADD, HEX);
        Serial.println("  !");
    } else {
        Serial.print("Unknown error at address 0x");
        Serial.println(TOUCH_I2C_ADD, HEX);
    }
}
