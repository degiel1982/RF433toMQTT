#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

void WiFiManager::setupWiFi() {
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print(F("Connecting to WiFi"));
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 10) {
    delay(500);
    Serial.print(F("."));
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println();
    Serial.print(F("WiFi connected, IP address: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println(F("WiFi connection failed. Please check credentials and retry."));
  }
}

void WiFiManager::printWiFiSignalStrength() {
  int32_t rssi = WiFi.RSSI();
  Serial.print(F("WiFi signal strength (RSSI): "));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
  WebSerial.print(F("WiFi signal strength (RSSI): "));
  WebSerial.print(rssi);
  WebSerial.println(F(" dBm"));
}

void WiFiManager::printWiFiSSID() {
  String ssidStr = WiFi.SSID();
  Serial.print(F("WiFi SSID: "));
  Serial.println(ssidStr);
  WebSerial.print(F("WiFi SSID: "));
  WebSerial.println(ssidStr);
}

void WiFiManager::printWiFiIP() {
  IPAddress ip = WiFi.localIP();
  Serial.print(F("WiFi IP address: "));
  Serial.println(ip);
  WebSerial.print(F("WiFi IP address: "));
  WebSerial.println(ip);
}
