#include "display_manager.h"
#include "mqtt_client.h"
#include <lvgl.h>
#include "ui.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup start");
    lcd_init();
    lvgl_init();
    ui_init();
    mqtt_init();
}

void loop()
{
    lv_timer_handler();
    mqtt_loop();
    delay(10);
}
