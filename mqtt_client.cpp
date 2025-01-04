#include "mqtt_client.h"
#include "display_manager.h"
#include "state_manager.h"
#include <lvgl.h>
#include "ui.h"

WiFiClient espClient;
PubSubClient internal_mqtt_client(espClient);
char msg[MQTT_BUFFER_LENGTH];
unsigned long retryTimer;
bool disconnected = true;
String clientId = "HVAC-Remote-";
char signalStrength[4];

void mqtt_init()
{
  connect_wifi();
  disconnected = false;
  Serial.print("Connected to WiFi with IP address: ");
  Serial.println(WiFi.localIP());
  randomSeed(micros());
  clientId += String(random(0xffff), HEX);
  retryTimer = millis();
  internal_mqtt_client.setServer(MY_MQTT_URL, 1883); // MY_MQTT_URL is in secrets.h
  internal_mqtt_client.setCallback(callback);
  if (!connect_mqtt(false))
  {
    reboot();
  }
  lv_scr_load(ui_main_screen);
}

void mqtt_loop()
{
  if (!WiFi.isConnected())
  {
    disconnected = true;
    reconnect_wifi();
  }
  bool mqttConnected = internal_mqtt_client.connected();
  if (!mqttConnected)
  {
    disconnected = true;
    reconnect_mqtt();
  }
  internal_mqtt_client.loop();
  unsigned long now = millis();
  if (now - retryTimer > MQTT_STATUS_UPDATE_TIME_MS)
  {
    retryTimer = now;
    if (disconnected)
    {
      disconnected = false;
      internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Reconnected");
    }
    else
    {
      internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Checkin");
    }
    update_signal();
    internal_mqtt_client.publish(MY_MQTT_SIGNAL_TOPIC, signalStrength);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char *output = reinterpret_cast<char *>(payload);
  if (strcmp(topic, MY_MQTT_IN_TOPIC) == 0)
  {
    update_state(output);
  }
  else if (strcmp(topic, MY_MQTT_TEMPERATURE_TOPIC) == 0)
  {
    update_temperature(output, length);
  }
  else if (strcmp(topic, MY_MQTT_THERMOSTAT_TOPIC) == 0)
  {
    update_thermostat(output, length);
  }
  else if (strcmp(topic, MY_MQTT_DOOR_TOPIC) == 0)
  {
    update_door(output);
  }
  else if (strcmp(topic, MY_MQTT_AVAILABLE_MODES_TOPIC) == 0)
  {
    update_available_modes(output, length);
  }
}

void connect_wifi()
{
  // MY_SSID and MY_PWD are defined in secrets.h which will not be uploaded
  // You will need to create your own version of this file to compile
  WiFi.setAutoReconnect(true);
  WiFi.begin(MY_SSID, MY_PWD);
  unsigned long start = millis();
  unsigned long now = 0;
  while (true)
  {
    now = millis();
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
    if (now - start >= WIFI_CONNECTION_TIMEOUT)
    {
      reboot();
    }
  }
}

void reconnect_wifi()
{
  if (ui_reconnect_screen != lv_scr_act())
  {
    lv_scr_load(ui_reconnect_screen);
    lv_timer_handler();
  }
  WiFi.disconnect();
  WiFi.reconnect();
  unsigned long start = millis();
  unsigned long now = 0;
  while (true)
  {
    now = millis();
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
    if (now - start >= WIFI_CONNECTION_TIMEOUT)
    {
      reboot();
    }
  }
  reset_screen_timer();
  lv_scr_load(ui_main_screen);
  lv_timer_handler();
}

bool connect_mqtt(bool isReconnect)
{
  uint8_t tries = 0;
  while (!internal_mqtt_client.connected())
  {
    if (internal_mqtt_client.connect(clientId.c_str()))
    {
      if (isReconnect)
      {
        internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Reconnected");
      }
      else
      {
        internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Connected");
      }
      internal_mqtt_client.subscribe(MY_MQTT_IN_TOPIC);
      internal_mqtt_client.subscribe(MY_MQTT_TEMPERATURE_TOPIC);
      internal_mqtt_client.subscribe(MY_MQTT_THERMOSTAT_TOPIC);
      internal_mqtt_client.subscribe(MY_MQTT_DOOR_TOPIC);
      internal_mqtt_client.subscribe(MY_MQTT_AVAILABLE_MODES_TOPIC);
    }
    else
    {
      if (++tries < MQTT_MAX_RETRIES)
      {
        delay(MQTT_RETRY_TIME);
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

void reconnect_mqtt()
{
  if (ui_reconnect_screen != lv_scr_act())
  {
    lv_scr_load(ui_reconnect_screen);
    lv_timer_handler();
  }
  if (!connect_mqtt(true))
  {
    reboot();
  }
  lv_scr_load(ui_main_screen);
  lv_timer_handler();
}

void mqtt_publish(char *publishMessage)
{
  snprintf(msg, MQTT_BUFFER_LENGTH, publishMessage);
  internal_mqtt_client.publish(MY_MQTT_OUT_TOPIC, msg);
}

void(* resetDevice) (void) = 0;

void reboot() {
  lv_label_set_text(ui_reconnect_label, "Unable to connect to WiFi.\nRestarting in 1 minute.");
  lv_timer_handler();
  delay(60000);
  resetDevice();
}

void update_signal()
{
  int dBm = WiFi.RSSI();
  if (dBm <= -100)
  {
    itoa(0, signalStrength, 10);
  }
  else if (dBm >= -50)
  {
    itoa(100, signalStrength, 10);
  }
  else
  {
    itoa(2 * (dBm + 100), signalStrength, 10);
  }
}