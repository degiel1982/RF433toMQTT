#include "MainManager.h"

MainManager::MainManager(const char* wifiSSID, const char* wifiPassword,
                         const char* mqttServer, int mqttPort,
                         const char* mqttUsername, const char* mqttPassword,
                         const char* deviceName, const char* deviceTopic, const char* deviceTopicStatic,
                         int rfPin, unsigned long debounce)
    : wifiManager(wifiSSID, wifiPassword),
      mqttManager(mqttServer, mqttPort, mqttUsername, mqttPassword, deviceName, deviceTopic, deviceTopicStatic),
      rfPin(rfPin),
      debounce(debounce) {}

void MainManager::setup() {
  WiFi.setSleep(WIFI_PS_NONE);
  Serial.begin(115200);

  wifiManager.setupWiFi();
  mqttManager.setupMQTT();
  rf433.setup(rfPin);
  webSerialManager.setupWebSerial();
}

void MainManager::loop() {
  if (!wifiManager.isConnected()) {
    wifiManager.setupWiFi();
  }

  if (!mqttManager.isConnected()) {
    mqttManager.reconnectMQTT();
  }

  mqttManager.loop();

  unsigned long rfcode = rf433.received_code(debounce);
  if (rfcode != 0 && rfcode > 1000 && mqttManager.isConnected()) {
    char rfcodeStr[10];
    snprintf(rfcodeStr, sizeof(rfcodeStr), "%lu", rfcode);
    mqttManager.publishAndLog(DEVICE_TOPIC, rfcodeStr);
    delay(10);
    mqttManager.publishAndLog(DEVICE_TOPIC_STATIC, rfcodeStr);
    delay(10);
    mqttManager.publishAndLog(DEVICE_TOPIC, "0");
  }

  if (Serial.available() > 0) {
    char command[50];
    size_t len = Serial.readBytesUntil('\n', command, sizeof(command) - 1);
    command[len] = '\0';
    trimWhitespace(command);
    if (strcmp(command, "wifi") == 0) {
      wifiManager.printWiFiSignalStrength();
    } else if (strcmp(command, "ssid") == 0) {
      wifiManager.printWiFiSSID();
    } else if (strcmp(command, "ip") == 0) {
      wifiManager.printWiFiIP();
    } else {
      WebSerial.println(F("Unknown command received."));
    }
  }
}
