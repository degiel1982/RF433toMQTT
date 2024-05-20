#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
public:
  WiFiManager(const char* ssid, const char* password);
  void setupWiFi();
  void printWiFiSignalStrength();
  void printWiFiSSID();
  void printWiFiIP();

private:
  const char* ssid;
  const char* password;
  bool wifiConnected = false;
};

#endif // WIFI_MANAGER_H
