#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>

class MQTTManager {
public:
  MQTTManager(const char* server, int port, const char* username, const char* password, const char* deviceName, const char* deviceTopic, const char* deviceTopicStatic);
  void setupMQTT();
  void reconnectMQTT();
  void publishAndLog(const char* topic, const char* payload);

  bool isConnected() { return mqttConnected && client.connected(); }

private:
  const char* server;
  int port;
  const char* username;
  const char* password;
  const char* deviceName;
  const char* deviceTopic;
  const char* deviceTopicStatic;
  unsigned long lastReconnectAttempt = 0;
  const unsigned long RECONNECT_INTERVAL = 5000;
  bool mqttConnected = false;
  WiFiClient espClient;
  PubSubClient client;
};

#endif // MQTT_MANAGER_H
