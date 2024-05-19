#include <RCSwitch.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

AsyncWebServer server(80);

class RF433 {
public:
  void setup(int receiver_pin = 0) {
    if (receiver_pin != 0) {
      rf433.enableReceive(receiver_pin);
    }
  }

  unsigned long received_code(unsigned long debounce_time = 0) {
    if (rf433.available()) {
      unsigned long rfcode = rf433.getReceivedValue();
      if (((millis() - old_time) > debounce_time) || rfcode != old_code) {
        old_code = rfcode;
        old_time = millis();
        rf433.resetAvailable();
        return rfcode;
      }
      rf433.resetAvailable();
    }
    return 0;
  }

private:
  unsigned long old_time = 0;
  unsigned long old_code = 0;
  RCSwitch rf433;
};

const unsigned long DEBOUNCE = 2000;
const int RF_PIN = 32;

// WiFi credentials
const char *SSID = "";
const char *PASSWORD = "";

// MQTT broker settings
const char *MQTT_SERVER = "";
const int MQTT_PORT = 1883;
const char *MQTT_USERNAME = "";
const char *MQTT_PASSWORD = "";

// Device settings
const char *DEVICE_NAME = "RF433Receiver";
const char *DEVICE_TOPIC = "rf433/receiver";
const char *DEVICE_TOPIC_STATIC = "rf433/receiver_static";

RF433 rf433;

WiFiClient espClient;
PubSubClient client(espClient);

bool wifiConnected = false;
bool mqttConnected = false;

unsigned long lastReconnectAttempt = 0;
const unsigned long RECONNECT_INTERVAL = 5000; // Try to reconnect every 5 seconds

void recvMsg(uint8_t *data, size_t len) {
  char message[256];
  size_t messageLen = len < sizeof(message) - 1 ? len : sizeof(message) - 1;
  memcpy(message, data, messageLen);
  message[messageLen] = '\0';

  WebSerial.print(F("Received Data: "));
  WebSerial.println(message);

  // Handle received commands
  if (strcmp(message, "wifi") == 0) {
    printWiFiSignalStrength();
  } else if (strcmp(message, "ssid") == 0) {
    printWiFiSSID();
  } else if (strcmp(message, "ip") == 0) {
    printWiFiIP();
  } else {
    WebSerial.println(F("Unknown command received."));
  }
}

void setupWiFi() {
  // Set WiFi mode and connect to network
  delay(500);
  WiFi.mode(WIFI_STA); // Set WiFi mode to station mode (client)
  WiFi.begin(SSID, PASSWORD); // Connect to WiFi network using SSID and password

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

void setup() {
  // Disable WiFi sleep modes
  WiFi.setSleep(WIFI_PS_NONE);

  // Initialize serial communication for debugging
  Serial.begin(115200);

  setupWiFi();

  // Connect to MQTT broker
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setKeepAlive(60); // Set MQTT keep alive to 60 seconds

  // Attempt to connect to MQTT
  reconnectMQTT();
  
  // Initialize RF433 module
  rf433.setup(RF_PIN);

  // Initialize WebSerial and start server
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
}

void loop() {
  // Check and reconnect WiFi if necessary
  if (!wifiConnected) {
    setupWiFi();
  }

  // Reconnect MQTT if not connected or connection lost
  if (!mqttConnected || !client.connected()) {
    reconnectMQTT();
  }

  // MQTT client loop
  client.loop();

  // RF433 main code
  unsigned long rfcode = rf433.received_code(DEBOUNCE);
  if (rfcode != 0 && rfcode > 1000 && mqttConnected && client.connected()) {
    char rfcodeStr[10];
    snprintf(rfcodeStr, sizeof(rfcodeStr), "%lu", rfcode);
    publishAndLog(DEVICE_TOPIC, rfcodeStr);
    delay(10); // Delay between publishes
    publishAndLog(DEVICE_TOPIC_STATIC, rfcodeStr);
    delay(10); // Delay between publishes
    publishAndLog(DEVICE_TOPIC, "0");
  }

  // Check if a command is received from serial monitor
  if (Serial.available() > 0) {
    char command[50];
    size_t len = Serial.readBytesUntil('\n', command, sizeof(command) - 1);
    command[len] = '\0';
    trimWhitespace(command);
    if (strcmp(command, "wifi") == 0) {
      printWiFiSignalStrength();
    } else if (strcmp(command, "ssid") == 0) {
      printWiFiSSID();
    } else if (strcmp(command, "ip") == 0) {
      printWiFiIP();
    } else {
      WebSerial.println(F("Unknown command received."));
    }
  }
}

void reconnectMQTT() {
  // If it's not time to attempt reconnection, return
  if (millis() - lastReconnectAttempt < RECONNECT_INTERVAL) {
    return;
  }

  lastReconnectAttempt = millis();

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (client.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      mqttConnected = true;
      Serial.println(F("connected"));
      // Once connected, publish an initial message
      publishAndLog(DEVICE_TOPIC, "RF433Receiver connected");
      publishAndLog(DEVICE_TOPIC_STATIC, "RF433Receiver connected");
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" retrying in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishAndLog(const char *topic, const char *payload) {
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

void printWiFiSignalStrength() {
  int32_t rssi = WiFi.RSSI();
  Serial.print(F("WiFi signal strength (RSSI): "));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
  WebSerial.print(F("WiFi signal strength (RSSI): "));
  WebSerial.print(rssi);
  WebSerial.println(F(" dBm"));
}

void printWiFiSSID() {
  String ssid = WiFi.SSID();
  Serial.print(F("WiFi SSID: "));
  Serial.println(ssid);
  WebSerial.print(F("WiFi SSID: "));
  WebSerial.println(ssid);
}

void printWiFiIP() {
  IPAddress ip = WiFi.localIP();
  Serial.print(F("WiFi IP address: "));
  Serial.println(ip);
  WebSerial.print(F("WiFi IP address: "));
  WebSerial.println(ip);
}

void trimWhitespace(char* str) {
  int len = strlen(str);
  while (len > 0 && isspace(str[len - 1])) {
    str[--len] = '\0';
  }
  while (*str && isspace(*str)) {
    str++;
  }
}
