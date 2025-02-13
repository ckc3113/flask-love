#include "DHT.h"
#include <WiFi.h>
#include <U8g2lib.h>

extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}

#include <AsyncMqttClient.h>

#define WIFI_SSID "Rosmita"
#define WIFI_PASSWORD "rosmitagyy"

#define MQTT_HOST "broker.hivemq.com"
#define MQTT_PORT 1883

#define MQTT_PUB_TEMP "temperature"
#define MQTT_PUB_HUM "humidity"
#define MQTT_SUB_LED "ledcontrol"

#define DHTPIN 22
#define OLEDPIN 19

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float temp;
float hum;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
TimerHandle_t sensorTimer;

const long interval = 10000;

U8G2_SSD1306_128X64_NONAME_1_4W_HWICR_AMPA u8g2(U8G2_R0);

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0);
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  mqttClient.subscribe(MQTT_SUB_LED, 1);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Message received on topic: " + String(topic));
  payload[len] = '\0';
  Serial.println("Payload: " + String(payload));

  if (strcmp(topic, MQTT_SUB_LED) == 0) {
    if (strcmp(payload, "on1") == 0) {
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_open_24px);
        u8g2.setCursor(0, 20);
        u8g2.print("LED: ON");
      } while (u8g2.nextPage());
    }
    else if (strcmp(payload, "off1") == 0) {
      u8g2.firstPage();
      do {
        u8g2.setFont(u8g2_font_open_24px);
        u8g2.setCursor(0, 20);
        u8g2.print("LED: OFF");
      } while (u8g2.nextPage());
    }
  }
}

void readSensor(TimerHandle_t xTimer) {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(temp) || isnan(hum)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEMP, packetIdPub1);
  Serial.printf("Message: %.2f \n", temp);

  uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());
  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
  Serial.printf("Message: %.2f \n", hum);

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_open_24px);
    u8g2.setCursor(0, 20);
    u8g2.print("Temperature: ");
    u8g2.print(String(temp));
    u8g2.print("°C");
    u8g2.setCursor(0, 50);
    u8g2.print("Humidity: ");
    u8g2.print(String(hum));
    u8g2.print("%");
  } while (u8g2.nextPage());
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  u8g2.begin();
  dht.begin();

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  sensorTimer = xTimerCreate("sensorTimer", pdMS_TO_TICKS(interval), pdTRUE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(readSensor));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWifi();

  xTimerStart(sensorTimer, 0); // Start the sensor timer
}

void loop() {
  // Empty because we're using FreeRTOS timers instead of the loop
}