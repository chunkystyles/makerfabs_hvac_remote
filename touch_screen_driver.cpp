#include "touch_screen_driver.h"

int readTouchReg(int reg)
{
    int data = 0;
    Wire.beginTransmission(TOUCH_I2C_ADD);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(TOUCH_I2C_ADD, 1);
    if (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}

/*
int getTouchPointX()
{
    int XL = 0;
    int XH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    XL = readTouchReg(TOUCH_REG_XL);

    return ((XH & 0x0F) << 8) | XL;
}
*/

int getTouchPointX()
{
    int XL = 0;
    int XH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    //Serial.println(XH >> 6,HEX);
    if(XH >> 6 == 1)
        return -1;
    XL = readTouchReg(TOUCH_REG_XL);

    return ((XH & 0x0F) << 8) | XL;
}

int getTouchPointY()
{
    int YL = 0;
    int YH = 0;

    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    return ((YH & 0x0F) << 8) | YL;
}

int ft6236_pos(int pos[2])
{
    int XL = 0;
    int XH = 0;
    int YL = 0;
    int YH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    if(XH >> 6 == 1)
    {
        pos[0] = -1;
        pos[1] = -1;
        return 0;
    }
    XL = readTouchReg(TOUCH_REG_XL);
    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    pos[0] = ((XH & 0x0F) << 8) | XL;
    pos[1] = ((YH & 0x0F) << 8) | YL;
    return 1;
}

int ft6236_coords(uint16_t *x, uint16_t *y)
{
    int XL = 0;
    int XH = 0;
    int YL = 0;
    int YH = 0;

    XH = readTouchReg(TOUCH_REG_XH);
    if(XH >> 6 == 1)
    {
        *x = -1;
        *y = -1;
        return 0;
    }
    XL = readTouchReg(TOUCH_REG_XL);
    YH = readTouchReg(TOUCH_REG_YH);
    YL = readTouchReg(TOUCH_REG_YL);

    *x = ((XH & 0x0F) << 8) | XL;
    *y = ((YH & 0x0F) << 8) | YL;
    return 1;
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t x, y;
  if (ft6236_coords(&x, &y)) {
    data->state = LV_INDEV_STATE_PR;
#if MY_SCREEN_ROTATION == 0
    data->point.x = x;
    data->point.y = y;
#endif

#if MY_SCREEN_ROTATION == 1
    data->point.x = y;
    data->point.y = MY_SCREEN_HEIGHT - x;
#endif

#if MY_SCREEN_ROTATION == 2
    data->point.x = MY_SCREEN_WIDTH - x;
    data->point.y = MY_SCREEN_HEIGHT - y;
#endif

#if MY_SCREEN_ROTATION == 3
    data->point.x = MY_SCREEN_WIDTH - y;
    data->point.y = x;
#endif

  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}