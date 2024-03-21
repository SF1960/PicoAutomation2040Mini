#pragma once

// Replace with your network credentials
const char* ssid = "TALKTALKB032AA";
const char* password = "6RGUAHGP";

// Variable to store the HTTP request
String header;

// Variable to store onboard LED state
String picoLEDState = "OFF";
String btnAState = "OFF";
String btnBState = "OFF";
String relayState = "OFF";

// Connection timeout variables
unsigned long currentTime = millis();    // Current time
unsigned long previousTime = 0;          // Previous time
const long timeoutTime = 10000;          // Define timeout time in milliseconds (example: 2000ms = 2s)

// Temperature variable
float temperature;                       // variable to hold Pico W temperature sensor data

// LED, Relay, Switches and Output Pins on Automation 2040 W Mini Board
#define LED_WIFI 3
#define LED_A 14
#define LED_B 15

#define RELAY 9

#define SWITCH_A 12
#define SWITCH_B 13

#define OUTPUT_1 16
#define OUTPUT_2 17

// Code replacement definitions
#define ButtonAPressed (digitalRead(SWITCH_A) == LOW)
#define ButtonBPressed (digitalRead(SWITCH_B) == LOW)
#define GetBoardTemperature (temperature = analogReadTemp())
