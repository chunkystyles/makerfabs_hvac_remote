This is a project using LVGL and Squareline Studio on the 3.5 inch parallel TFT ESP32-S3 device made by Makerfabs: https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-ili9488.html

The purpose of this project is to create a counter-top wireless controller for individual mini-split air conditioners.  The UI is built in Squareline Studio and is intended to be dead simple and only give the most basic functions needed.  It communicates over MQTT which I coordinate on the backend with Homeassistant and Node-RED.

I have created a 3d printable shell for the device that is designed to sit on a counter top at a 45 degree angle.  You can find the files here https://www.thingiverse.com/thing:6122076

I copied code from https://github.com/0015/ThatProject/blob/master/ESP32_LVGL/LVGL8_SquareLine/03_Let's_build_a_GPS_Speedometer/SC01Plus_BN880Q_GPS_App_Part2/SC01Plus_BN880Q_GPS_App_Part2.ino to get LVGL configured properly.