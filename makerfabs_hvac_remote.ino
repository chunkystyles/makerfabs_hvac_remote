#include "hardware_setup.h"
#include <lvgl.h>
#include "ui.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Setup start");
    lcd_init();
    lvgl_init();
    ui_init();
}

void loop() {
    lv_timer_handler();
    delay(5);
}
