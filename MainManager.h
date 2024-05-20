#ifndef MAIN_MANAGER_H
#define MAIN_MANAGER_H

#include "RF433.h"
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "WebSerialManager.h"

class MainManager {
public:
  MainManager(const char* wifiSSID, const char* wifiPassword,
              const char* mqttServer, int mqttPort,
              const char* mqttUsername, const char* mqttPassword,
              const char* deviceName, const char* deviceTopic, const char* deviceTopicStatic,
              int rfPin, unsigned long debounce);

  void setup();
  void loop();

private:
  RF433 rf433;
  WiFiManager wifiManager;
  MQTTManager mqttManager;
  WebSerialManager webSerialManager;
  const int rfPin;
  const unsigned long debounce;
};

#endif // MAIN_MANAGER_H
