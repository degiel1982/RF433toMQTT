#include "MainManager.h"

// Define your credentials and settings here
const char* SSID = "your_ssid";
const char* PASSWORD = "your_password";
const char* MQTT_SERVER = "your_mqtt_server";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "your_mqtt_username";
const char* MQTT_PASSWORD = "your_mqtt_password";
const char* DEVICE_NAME = "RF433Receiver";
const char* DEVICE_TOPIC = "rf433/receiver";
const char* DEVICE_TOPIC_STATIC = "rf433/receiver_static";
const int RF_PIN = 32;
const unsigned long DEBOUNCE = 2000;

MainManager mainManager(SSID, PASSWORD, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, DEVICE_NAME, DEVICE_TOPIC, DEVICE_TOPIC_STATIC, RF_PIN, DEBOUNCE);

void setup() {
  mainManager.setup();
}

void loop() {
  mainManager.loop();
}
