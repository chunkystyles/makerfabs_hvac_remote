#include "mqtt_client.h"
#include "hvac.h"

WiFiClient espClient;
PubSubClient internal_mqtt_client(espClient);
char msg[MQTT_BUFFER_LENGTH];
unsigned long timer;

void mqtt_init()
{
  // MY_SSID and MY_PWD are defined in secrets.h which will not be uploaded
  // You will need to create your own version of this file to compile
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
  Serial.print("Wifi init finished.");
  randomSeed(micros());
  timer = millis();
  internal_mqtt_client.setServer(MY_MQTT_URL, 1883); // MY_MQTT_URL is in secrets.h, needs to be a URL, IP Address uses a different constructor
  internal_mqtt_client.setCallback(callback);
  connect(false);
}

void mqtt_loop()
{
  if (!WiFi.isConnected())
  {
    Serial.println("WiFi disconnected");
    do
    {
      delay(5000);
      Serial.println("Attempting to reconnect WiFi");
    } while (!WiFi.reconnect());
  }
  if (!internal_mqtt_client.connected())
  {
    connect(true);
  }
  internal_mqtt_client.loop();
  unsigned long now = millis();
  if (now - timer > MQTT_STATUS_UPDATE_TIME_MS)
  {
    timer = now;
    internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Checkin");
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char *output = reinterpret_cast<char *>(payload);
  updateStateFromMqtt(output);
}

void connect(bool isReconnect)
{
  while (!internal_mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (internal_mqtt_client.connect(clientId.c_str()))
    {
      if (isReconnect)
      {
        Serial.println("reconnected");
        internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Reconnected");
      }
      else
      {
        Serial.println("connected");
        internal_mqtt_client.publish(MY_MQTT_STATUS_TOPIC, "Connected");
      }
      internal_mqtt_client.subscribe(MY_MQTT_IN_TOPIC);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(internal_mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_publish(char *publishMessage)
{
  snprintf(msg, MQTT_BUFFER_LENGTH, publishMessage);
  internal_mqtt_client.publish(MY_MQTT_OUT_TOPIC, msg);
}