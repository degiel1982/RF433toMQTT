### Description of RF433 Receiver for Home Assistant

The RF433 Receiver is an ESP32-based device designed to receive RF433 signals and integrate them with Home Assistant using MQTT. This project was born out of the need to use an RF receiver with Home Assistant, as existing solutions like ESPHome did not meet the requirements.

#### What It Does

The RF433 Receiver listens for RF433 MHz signals using a connected receiver module. When it detects a signal, it captures the unique code sent by the RF device and transmits this code to an MQTT broker. Home Assistant can then listen for these MQTT messages and trigger automations based on the received RF codes. This setup allows for seamless integration of RF433 devices (like remote controls, door sensors, etc.) with your smart home system.

#### Why It Is Used

The primary purpose of the RF433 Receiver is to bridge RF433 devices with Home Assistant, enabling automation and smart home functionality without the need for complex setups or proprietary hubs. By leveraging MQTT, a lightweight messaging protocol, the device ensures reliable communication and easy configuration.

#### Key Features

- **WiFi Connectivity**: The device connects to your home WiFi network, allowing it to communicate with the MQTT broker and Home Assistant.
- **MQTT Integration**: Sends received RF433 codes as MQTT messages to a specified topic, which Home Assistant can monitor.
- **Web Serial Interface**: Provides a web interface for real-time debugging and command execution.
- **Debounce Logic**: Prevents duplicate signals from being processed within a specified debounce interval, ensuring only distinct RF codes are sent.
- **Device Configuration**: Easily configurable WiFi and MQTT settings through the source code.

#### Code Details

- **Libraries Used**: The code utilizes several libraries including RCSwitch for RF signal processing, WiFi and AsyncTCP for network connectivity, PubSubClient for MQTT communication, and ESPAsyncWebServer and WebSerial for web-based interaction and debugging.
- **Setup Functions**: 
  - `setupWiFi()`: Connects the ESP32 to the WiFi network.
  - `reconnectMQTT()`: Ensures the device maintains a connection to the MQTT broker.
  - `setup()`: Initializes all components including WiFi, MQTT, RF receiver, and web server.
- **Main Loop**: The `loop()` function continuously checks for WiFi and MQTT connectivity, processes received RF signals, and publishes the RF codes to the MQTT broker.
- **MQTT Topics**:
  - `DEVICE_TOPIC`: For publishing received RF codes and then resetting the code to 0. This ensures Home Assistant registers the code as a change and triggers the corresponding automation.
  - `DEVICE_TOPIC_STATIC`: For maintaining and displaying the last received RF code.

#### Why Two MQTT Topics?

The RF433 Receiver uses two MQTT topics for effective communication with Home Assistant:

1. **Static Topic (`DEVICE_TOPIC_STATIC`)**: This topic holds the last received RF code, providing a consistent reference. It allows Home Assistant to know the most recently received code at any given time.
   
2. **Regular Topic (`DEVICE_TOPIC`)**: This topic is used to publish the received RF code and then quickly reset the value to 0. This is necessary because Home Assistant requires a state change to trigger automations. By resetting the value, any new RF code received will be recognized as a change, ensuring that Home Assistant reliably detects and responds to new signals.

### How to Change Settings

To change the WiFi or MQTT settings, you need to modify the respective values in the source code. Below are the settings you can change:

1. **WiFi Credentials**: 
   ```cpp
   const char *SSID = "your_wifi_ssid"; // Your WiFi SSID
   const char *PASSWORD = "your_wifi_password"; // Your WiFi Password
   ```

2. **MQTT Broker Settings**: 
   ```cpp
   const char *MQTT_SERVER = "your_mqtt_broker_ip"; // Your MQTT Broker IP Address
   const int MQTT_PORT = 1883;  // Your MQTT Broker Port (default is 1883)
   const char *MQTT_USERNAME = "your_mqtt_username"; // Your MQTT Broker Username
   const char *MQTT_PASSWORD = "your_mqtt_password"; // Your MQTT Broker Password
   ```

3. **Device Settings**: 
   ```cpp
   const int RF_PIN = 32; // The GPIO pin connected to the RF receiver
   const unsigned long DEBOUNCE = 2000; // Debounce time in milliseconds
   const char *DEVICE_NAME = "RF433Receiver"; // Device name for MQTT
   const char *DEVICE_TOPIC = "rf433/receiver"; // MQTT topic for received RF codes
   const char *DEVICE_TOPIC_STATIC = "rf433/receiver_static"; // MQTT topic for static display of RF codes
   ```

After modifying these settings, upload the updated code to your ESP32 device using the Arduino IDE or another compatible environment.

### Example Usage

1. **Connect the RF433 Receiver**: Power up your ESP32 device and ensure it is connected to your WiFi network.
2. **Configure Home Assistant**: Set up MQTT integration in Home Assistant and create automations based on the topics `rf433/receiver` and `rf433/receiver_static`.
3. **Trigger RF Signals**: Use an RF remote or any RF433 device to send signals, which will be captured and transmitted by the RF433 Receiver.
4. **Monitor and Automate**: Home Assistant will receive the RF codes via MQTT and execute the corresponding automations you have set up.

### Example Usage (Continued)

1. **Connect the RF433 Receiver**: 
   - Ensure your ESP32 device is powered and connected to your WiFi network by verifying the serial output during startup.
   - The device will attempt to connect to the configured WiFi network and print the IP address once connected.

2. **Configure Home Assistant**: 
   - Add MQTT integration in Home Assistant and subscribe to the topics `rf433/receiver` and `rf433/receiver_static`.
   - Create automations that respond to the RF codes received on these topics. For example:
     ```yaml
     automation:
       - alias: "RF433 Signal Received"
         trigger:
           platform: mqtt
           topic: "rf433/receiver"
           payload: "123456"  # Replace with your RF code
         action:
           - service: light.toggle
             entity_id: light.your_light_entity
     ```

3. **Trigger RF Signals**: 
   - Use an RF remote or any RF433 device to send signals.
   - The RF433 Receiver will capture these signals, process the RF code, and publish it to the MQTT broker.
   - The code is published to `rf433/receiver` and `rf433/receiver_static`.

4. **Monitor and Automate**: 
   - Home Assistant listens for messages on the MQTT topics and triggers the corresponding automations.
   - You can monitor the received RF codes via the Web Serial interface or the Home Assistant logs to ensure everything is working as expected.

### Additional Information

- **Web Serial Interface**: 
  - Accessible via the IP address of the ESP32 on port 80. 
  - Allows real-time monitoring and debugging of RF codes and WiFi status.
  - Commands like "wifi", "ssid", and "ip" can be sent via the Web Serial interface to get information about the device's WiFi connection.

- **Reconnection Logic**: 
  - The device includes logic to automatically reconnect to the WiFi network and MQTT broker if the connection is lost.
  - It attempts to reconnect every 5 seconds, ensuring continuous operation without manual intervention.

### Troubleshooting

- **WiFi Connection Issues**: 
  - Ensure the SSID and password are correctly set in the code.
  - Check the signal strength and position of your WiFi router.
  - Monitor the serial output to see connection attempts and potential error messages.

- **MQTT Connection Issues**: 
  - Verify the MQTT broker settings (IP address, port, username, and password) are correct.
  - Ensure the MQTT broker is running and accessible from the ESP32.
  - Use tools like MQTT Explorer to verify that messages are being received on the configured topics.

- **RF Signal Reception**: 
  - Ensure the RF receiver module is properly connected to the ESP32 on the specified GPIO pin.
  - Test with different RF devices to verify the signal reception.
  - Adjust the debounce time if you encounter issues with duplicate signals.

This project simplifies the integration of RF433 devices with Home Assistant, offering a customizable and user-friendly solution for smart home enthusiasts. By following the steps above, you can easily set up and configure the RF433 Receiver to enhance your home automation system.

