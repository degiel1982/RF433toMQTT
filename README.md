# RF433toMQTT
### RF433 Receiver Device User Manual for Home Assistant Integration

**Introduction:**
Welcome to the user manual for your RF433 Receiver device! This device is designed to receive signals from 433 MHz RF (Radio Frequency) transmitters and integrate them with your Home Assistant setup using MQTT (Message Queuing Telemetry Transport). It also provides a web-based interface for easy control and monitoring.

**Setup:**
1. **Hardware Setup:**
   - Connect your RF receiver module to the ESP32 development board. The RF receiver is connected to GPIO pin 32 (RF_PIN) by default. Adjust the wiring if you change the pin.

2. **WiFi Connection:**
   - Power on the device. It will attempt to connect to the WiFi network specified in the code (`SSID` and `PASSWORD`). The device's IP address will be displayed on the Serial Monitor once it's connected.

3. **MQTT Broker Configuration:**
   - Configure the MQTT broker settings (`MQTT_SERVER`, `MQTT_PORT`, `MQTT_USERNAME`, `MQTT_PASSWORD`) to match your MQTT broker's credentials.

4. **Home Assistant Integration:**
   - Ensure that Home Assistant is set up and running.
   - Add the MQTT broker integration to Home Assistant if you haven't already:
     - Navigate to Configuration -> Integrations in the Home Assistant UI.
     - Click on the "+" icon to add an integration.
     - Search for "MQTT" and follow the instructions to add your MQTT broker.
     - Use the MQTT settings configured in your RF433 Receiver device (`MQTT_SERVER`, `MQTT_PORT`, `MQTT_USERNAME`, `MQTT_PASSWORD`).

**Usage:**
- Once the device is set up and connected, you can interact with it through the following methods:

1. **Web Interface:**
   - Open a web browser and enter the IP address of the device. You will see a simple web page.
   - Use the web page to execute commands:
     - "wifi" - Displays WiFi signal strength (RSSI).
     - "ssid" - Displays the SSID of the connected WiFi network.
     - "ip" - Displays the local IP address of the device.

2. **Serial Interface:**
   - Open the Serial Monitor in the Arduino IDE (or other serial communication software).
   - Send commands:
     - "wifi" - Displays WiFi signal strength (RSSI).
     - "ssid" - Displays the SSID of the connected WiFi network.
     - "ip" - Displays the local IP address of the device.

3. **RF433 Signals:**
   - The device listens for RF signals at 433 MHz.
   - When a valid signal is received, it is processed and then published to the MQTT broker under the topics `rf433/receiver` and `rf433/receiver_static`.

4. **Home Assistant Automation:**
   - In Home Assistant, create automations and scripts based on the received RF signals:
     - **Automations:** Set up automations to trigger actions when specific RF signals are received. For example, turn on a light when a particular remote button is pressed.
     - **Scripts:** Use scripts to perform complex actions or sequences based on RF signal inputs.
     - **MQTT Integration:** Home Assistant will automatically detect the MQTT topics published by the RF433 Receiver. You can use these topics as triggers for your automations and scripts.

**Use Case:**
Imagine you have several RF remote controls for various devices in your home, such as lights, garage doors, and fans. You want to integrate these RF devices into your Home Assistant smart home setup for better control and automation.

1. **Control via Web Interface:**
   - Use the web interface of the RF433 Receiver device to check WiFi signal strength, SSID, and IP address.
   - Monitor the RF signals received by the device through the web interface.

2. **Integration with Home Assistant:**
   - Configure Home Assistant to subscribe to the MQTT topics published by the RF433 Receiver.
   - Create automations and scripts in Home Assistant to respond to RF signals. For example:
     - Turn on lights when a specific button on an RF remote control is pressed.
     - Open garage doors when another button is pressed.
     - Adjust fan speeds when yet another button is pressed.

3. **Monitoring and Automation:**
   - Monitor the RF signals through Home Assistant's MQTT integration.
   - Automate tasks based on RF signal inputs to enhance the convenience and efficiency of your smart home.

**Conclusion:**
The RF433 Receiver device offers a seamless integration solution for your existing RF remote controls with your Home Assistant smart home setup. It provides an intuitive web interface for easy configuration and monitoring, as well as MQTT integration for advanced automation and control capabilities.

For any further assistance or customization, please refer to the provided code or contact support.

Enjoy enhancing your smart home experience with the RF433 Receiver!

**End of User Manual.**
