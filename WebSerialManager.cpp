#include "WebSerialManager.h"

void recvMsg(uint8_t* data, size_t len) {
  char message[256];
  size_t messageLen = len < sizeof(message) - 1 ? len : sizeof(message) - 1;
  memcpy(message, data, messageLen);
  message[messageLen] = '\0';

  WebSerial.print(F("Received Data: "));
  WebSerial.println(message);

  // Handle received commands
  if (strcmp(message, "wifi") == 0) {
    // Implement your WiFiManager instance here to call appropriate methods
  } else if (strcmp(message, "ssid") == 0) {
    // Implement your WiFiManager instance here to call appropriate methods
  } else if (strcmp(message, "ip") == 0) {
    // Implement your WiFiManager instance here to call appropriate methods
  } else {
    WebSerial.println(F("Unknown command received."));
  }
}

WebSerialManager::WebSerialManager() : server(80) {}

void WebSerialManager::setupWebSerial() {
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
}
