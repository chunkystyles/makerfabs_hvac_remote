#include "mqtt_client.h"
#include "state_manager.h"

WiFiClient espClient;
PubSubClient internal_mqtt_client(espClient);
char msg[MQTT_BUFFER_LENGTH];
unsigned long retryTimer;
bool disconnected = true;

void mqtt_init()
{
  // MY_SSID and MY_PWD are defined in secrets.h which will not be uploaded
  // You will need to create your own version of this file to compile
  WiFi.setAutoConnect(true);
  WiFi.begin(MY_SSID, MY_PWD);
  int connect_count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    connect_count++;
    if (connect_count > 20)
    {
      Serial.println("Wifi time out");
      break;
    }
  }
  disconnected = false;
  Serial.println("Wifi init finished.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  randomSeed(micros());
  retryTimer = millis();
  internal_mqtt_client.setServer(MY_MQTT_URL, 1883); // MY_MQTT_URL is in secrets.h
  internal_mqtt_client.setCallback(callback);
  connect(false);
}

void mqtt_loop()
{
  if (!WiFi.isConnected())
  {
    disconnected = true;
    // TODO: create screen to display disconnected
    return;
  }
  bool mqttConnected = internal_mqtt_client.connected();
  if (!mqttConnected)
  {
    mqttConnected = connect(true);
  }
  if (mqttConnected)
  {
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
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char *output = reinterpret_cast<char *>(payload);
  if (strcmp(topic, MY_MQTT_IN_TOPIC) == 0)
  {
    updateStateFromMqtt(output);
  }
  else if (strcmp(topic, MY_MQTT_TEMPERATURE_TOPIC) == 0)
  {
    updateTemperatureFromMqtt(output);
  }
  else if (strcmp(topic, MY_MQTT_DOOR_TOPIC) == 0)
  {
    updateDoorFromMqtt(output);
  }
}

bool connect(bool isReconnect)
{
  uint8_t tries = 0;
  while (!internal_mqtt_client.connected())
  {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
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
      internal_mqtt_client.subscribe(MY_MQTT_DOOR_TOPIC);
    }
    else
    {
      if (++tries < MQTT_MAX_RETRIES)
      {
        Serial.print("failed, rc=");
        Serial.print(internal_mqtt_client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

void mqtt_publish(char *publishMessage)
{
  snprintf(msg, MQTT_BUFFER_LENGTH, publishMessage);
  internal_mqtt_client.publish(MY_MQTT_OUT_TOPIC, msg);
}