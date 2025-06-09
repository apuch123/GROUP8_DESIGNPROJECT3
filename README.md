# GROUP8_DESIGNPROJECT3

IoT Environmental Monitoring and Control with ESP32 and ESP RainMaker

1. Project Overview
This capstone design project tasks you with developing a connected IoT system utilizing the ESP32 microcontroller and the ESP RainMaker platform. Your system will monitor environmental parameters using sensors and control connected actuators remotely via a mobile application and the RainMaker cloud backend. You will integrate hardware components, implement firmware using the ESP-IDF and RainMaker SDK, provision devices to the cloud, and demonstrate core RainMaker functionalities including real-time data logging, scheduling, automations, and Over-the-Air (OTA) firmware updates. This project emphasizes practical skills in embedded firmware development, cloud integration via a managed service (RainMaker), mobile app interaction, and understanding the lifecycle of an IoT device.

2. Learning Objectives
Upon successful completion of this project, you will be able to:
•	Firmware Development: Configure the ESP32 development environment using ESP-IDF, integrate the ESP RainMaker SDK, compile, flash, and debug complex embedded firmware.

•	Cloud Integration: Understand and utilize the ESP RainMaker platform for device provisioning, certificate management, secure communication (MQTT), and remote device control via the RainMaker dashboard and APIs.

•	Mobile App Interaction: Use the standard RainMaker Phone App for device Wi-Fi provisioning, user association, device management, and remote control of parameters and attributes.

•	Sensor & Actuator Interfacing: Integrate common environmental sensors (e.g., temperature, humidity) and digital actuators (e.g., relays) into the RainMaker data model, ensuring reliable data acquisition and control.

•	Advanced Features: Implement and configure higher-level IoT features such as scheduling, scenes, and automations (e.g., trigger an action based on sensor thresholds) using the RainMaker framework.

•	OTA Updates: Develop and demonstrate a functional Over-the-Air (OTA) firmware update mechanism using the RainMaker cloud infrastructure, a crucial feature for managing deployed IoT devices.

•	System Integration & Troubleshooting: Design, build, and troubleshoot an end-to-end IoT system, identifying and resolving issues across hardware, firmware, network connectivity, and cloud services.

4. Project Requirements
Your final system must meet the following requirements:
•	Connect an ESP32 board to Wi-Fi and the ESP RainMaker cloud service.

•	Successfully provision the device using the standard RainMaker Phone App (QR code or SoftAP).

•	Integrate at least two (2) different environmental sensors (e.g., temperature, humidity, light, pressure) and publish their readings to RainMaker as time-series data.

•	Integrate at least one (1) actuator (e.g., relay controlling an LED or fan, or a controllable light) that can be controlled remotely via the RainMaker app.
•	Define custom parameters and attributes in the RainMaker data model for your sensors and actuators.

•	Demonstrate real-time sensor data visualization and actuator control via the RainMaker app.

•	Implement and demonstrate an automation scenario using RainMaker, triggered by sensor data (e.g., if temperature > X, turn on the actuator; if humidity > Y, turn on the actuator).

•	Implement and demonstrate a simple scheduling scenario via RainMaker (e.g., turn on/off the actuator at a specific time).

•	Implement a functional Over-the-Air (OTA) update mechanism and demonstrate updating the firmware remotely via the RainMaker cloud.

•	The firmware must be built upon the latest stable or master branch of ESP-IDF and the integrated ESP RainMaker SDK.

•	All essential project configurations (Wi-Fi credentials placeholder, RainMaker credentials, etc.) must be handled securely and not hardcoded in a way that compromises security if the code is shared.

6. Hardware Requirements
You will need the following hardware components:
•	ESP32 DevKit Board: Any common ESP32, ESP32-S2, ESP32-S3, or ESP32-C3 development board (e.g., ESP32-WROOM-32 based boards, ESP32-S3-DevKitC).
•	Environmental Sensors: At least two distinct types.
Examples: 
	DHT22 (Temperature & Humidity) or DHT11
	BMP180/BMP280 (Pressure & Temperature)
	BH1750 (Ambient Light Intensity)
	MQ-series gas sensor (requires ADC and likely calibration, potentially more complex)

•	Actuator Module: At least one controllable channel. Examples: 
	Single or Multi-channel 5V Relay Module (ensure it has isolation if controlling AC loads - for this project, controlling a simple LED or small DC fan via the relay is sufficient and safer).
	Addressable LED strip (WS2812B/NeoPixel).
Small DC motor with driver (e.g., L298N or simple transistor circuit).

•	Prototyping Supplies: Breadboard, jumper wires (male-male, male-female), micro USB or USB-C cable for the ESP32 board.

•	Power Supply: A suitable 5V power supply for the relay module if it requires external power beyond the ESP32's GPIO capability.

7. Software & Services Requirements

You will need access to and familiarity with the following software and services:
•	ESP-IDF: The official Espressif IoT Development Framework. You should install the environment according to Espressif's documentation. Use a recent stable release or the master branch.

•	ESP RainMaker SDK: Integrated within ESP-IDF examples, but familiarity with cloning and managing it alongside your project is recommended.

•	C/C++ Programming Environment: A code editor or IDE configured for ESP-IDF development (e.g., VS Code with the Espressif IDF extension, Eclipse).

•	ESP RainMaker CLI: (Recommended, not strictly required) Useful for interacting with the RainMaker cloud backend from your computer for debugging and configuration verification.

•	ESP RainMaker Phone App: (Required) Available on Android and iOS app stores. This is essential for provisioning, user binding, and control.

•	ESP RainMaker Cloud Service: You will use the public RainMaker cloud service for this project. An AWS account is not required unless you choose to deploy a private instance (which is beyond the scope of the basic requirements for this project).

10. Proposed Tasks & Milestones

Here is a suggested breakdown of the project into logical phases. Focus on completing the tasks within each phase before moving to the next.
•	Phase 1: Development Environment Setup and Basic Connectivity

Install ESP-IDF and configure your development environment.
Clone the ESP RainMaker examples repository or configure your project to include RainMaker.
Build and flash a basic RainMaker example (like the switch or hello_world example) to your ESP32 board.
Successfully provision the example device using the RainMaker Phone App and verify basic connectivity and control (e.g., toggling an LED if the example supports it).

•	Phase 2: Sensor Data Integration
Select and connect your chosen sensors to the ESP32 according to their datasheets.
Write or integrate sensor driver code to read data reliably (handling necessary communication protocols like I2C, SPI, or basic GPIO).
Modify the RainMaker example firmware or create a new project based on a RainMaker template.
Define RainMaker parameters/attributes for your sensor readings (e.g., temperature, humidity, light).
Modify the firmware to periodically read sensor data and update the corresponding RainMaker parameters.
Verify that sensor data is being logged and is visible in the RainMaker Phone App and potentially the RainMaker dashboard.

•	Phase 3: Actuator Control and Automation
Select and connect your chosen actuator module to the ESP32, ensuring appropriate power and control signals.
Write driver code to control the actuator (e.g., turn relay on/off, set LED color/brightness).
Define a RainMaker parameter/attribute for your actuator (e.g., power (on/off), brightness, color).
Modify the firmware to respond to changes in the actuator's RainMaker parameter (remote control via app).
Using the RainMaker Phone App or CLI, configure an automation that uses one of your sensor readings as a trigger and controls the actuator as the action (e.g., Humidity > 60% -> turn Relay ON).
Using the RainMaker Phone App or CLI, configure a simple schedule for the actuator (e.g., Turn Relay ON at 8:00 AM, OFF at 5:00 PM).
Demonstrate both the automation and the schedule functioning correctly without direct app intervention.

•	Phase 4: OTA Updates and Advanced Feature Implementation
Configure your ESP-IDF project to support OTA updates via RainMaker.
Create a slightly modified version of your firmware (e.g., change an LED blink pattern, add a small message printout, adjust a sensor reading interval) to serve as the update image.
Generate the OTA binary file.
Using the RainMaker Phone App or CLI, initiate an OTA update to push the new firmware to your device.
Verify that the device successfully receives, flashes, and boots the new firmware, and that the modified behavior is present.
Implement and demonstrate at least one additional advanced RainMaker feature (e.g., Node Sharing, Device Grouping, Push Notifications - note: Push Notifications require Firebase setup, which adds complexity; Node Sharing/Grouping might be simpler to demonstrate within RainMaker).

•	Phase 5: Documentation and Finalization

Clean up your firmware source code, add comments, and ensure it follows good coding practices.
Prepare your project report detailing your design choices, implementation steps, challenges encountered, and results.
Prepare for your final demonstration. Ensure all components are working reliably and you can showcase each required feature

