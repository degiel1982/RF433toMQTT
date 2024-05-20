#ifndef WEBSERIAL_MANAGER_H
#define WEBSERIAL_MANAGER_H

#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

void recvMsg(uint8_t* data, size_t len);

class WebSerialManager {
public:
  WebSerialManager();
  void setupWebSerial();

private:
  AsyncWebServer server;
};

#endif // WEBSERIAL_MANAGER_H
