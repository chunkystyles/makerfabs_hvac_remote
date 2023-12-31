#include <Wire.h>
#include <lvgl.h>
#include "display_settings.h"

#define TOUCH_I2C_ADD 0x38
#define TOUCH_REG_XL 0x04
#define TOUCH_REG_XH 0x03
#define TOUCH_REG_YL 0x06
#define TOUCH_REG_YH 0x05

int read_touch_reg(int reg);

int ft6236_coords(uint16_t *x, uint16_t *y);

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);