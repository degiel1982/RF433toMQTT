#include "MQTTManager.h"

MQTTManager::MQTTManager(const char* server, int port, const char* username, const char* password,
                         const char* deviceName, const char* deviceTopic, const char* deviceTopicStatic)
    : server(server), port(port), username(username), password(password),
      deviceName(deviceName), deviceTopic(deviceTopic), deviceTopicStatic(deviceTopicStatic),
      client(espClient) {}

void MQTTManager::setupMQTT() {
  client.setServer(server, port);
  client.setKeepAlive(60);
  reconnectMQTT();
}

void MQTTManager::reconnectMQTT() {
  if (millis() - lastReconnectAttempt < RECONNECT_INTERVAL) {
    return;
  }

  lastReconnectAttempt = millis();

  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    if (client.connect(deviceName, username, password)) {
      mqttConnected = true;
      Serial.println(F("connected"));
      publishAndLog(deviceTopic, "RF433Receiver connected");
      publishAndLog(deviceTopicStatic, "RF433Receiver connected");
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" retrying in 5 seconds"));
      delay(5000);
    }
  }
}

void MQTTManager::publishAndLog(const char* topic, const char* payload) {
  if (client.publish(topic, payload)) {
    Serial.print(F("Published to MQTT topic: "));
    Serial.println(topic);
    Serial.print(F("  Payload: "));
    Serial.println(payload);
    WebSerial.print(F("Published to MQTT topic: "));
    WebSerial.println(topic);
    WebSerial.print(F("  Payload: "));
    WebSerial.println(payload);
  } else {
    Serial.print(F("Failed to publish to MQTT topic: "));
    Serial.println(topic);
    WebSerial.print(F("Failed to publish to MQTT topic: "));
    WebSerial.println(topic);
  }
}
