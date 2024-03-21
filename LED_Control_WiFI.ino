/*
  Pico W Web Interface Demo
  picow-web-control-demo.ino
  Web Interface & WiFi Connection
  Control the onboard LED with Pico W

  Adapted from ESP32 example by Rui Santos - https://randomnerdtutorials.com

  DroneBot Workshop 2022
  https://dronebotworkshop.com


  Updated to use Automation 2040 W Mini board by sgfpcb@gmail.com
  LED_Control_WiFi.ino
  Control the relay, the 2 outputs and the on-board LED from a Web Page

  March 2024

*/

// Load Wi-Fi library
#include <WiFi.h>
#include "globals.h"
#include "automation_board.h"
#include "WiFiHelper.h"

// Set web server port number to 80
WiFiServer server(80);

// Set up program to run
void setup() {

  // Start Serial Monitor
  Serial.begin(115200);

  boardSetup::setupPins();
  boardSetup::initPins();
  wifiSetup::connect();

  // Start Server
  server.begin();
}

// Main program loop
void loop() {

  // check PB presses
  // button A pressed
  //if(digitalRead(SWITCH_A) == LOW){ 
  if ButtonAPressed {  
    if (btnAState == "ON"){
      digitalWrite(LED_A, LOW);
      digitalWrite(OUTPUT_1, LOW);  
      btnAState = "OFF";  
      delay(500);

    } else {
      digitalWrite(LED_A, HIGH);
      digitalWrite(OUTPUT_1, HIGH);  
      btnAState = "ON"; 
      delay(500); 
    }

  } 

  // button B pressed
  if ButtonBPressed { 
    if (btnBState == "ON"){
      digitalWrite(LED_B, LOW);
      digitalWrite(OUTPUT_2, LOW);  
      btnBState = "OFF";  
      delay(500);

    } else {
      digitalWrite(LED_B, HIGH);
      digitalWrite(OUTPUT_2, HIGH);  
      btnBState = "ON"; 
      delay(500); 
    }

  } 

  GetBoardTemperature;                                           // Get internal temperature from the Pico W

  WiFiClient client = server.available();                        // Listen for incoming clients

  if (client) {                                                  // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");                               // print a message out in the serial port
    String currentLine = "";                                     // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {                                  // if there's bytes to read from the client,
        char c = client.read();                                  // read a byte, then
        Serial.write(c);                                         // print it out the serial monitor
        header += c;
        if (c == '\n') {                                         // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Refresh: 4;URL='//192.168.1.34:80/'>"); // refresh browser every 4 seconds           
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Switch the outputs on and off
            // LED
            Serial.println("**************** HEADER *********************");
            Serial.println(header);
            Serial.println("***************** END ***********************");

            // Go through header string and find what has been returned
            if (header.indexOf("GET /led/on") >= 0) {
              Serial.println("LED on");
              picoLEDState = "ON";
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /led/off") >= 0) {
              Serial.println("LED off");
              picoLEDState = "OFF";
              digitalWrite(LED_BUILTIN, LOW);

            // Btn A sets Output 1
            } else if (header.indexOf("GET /btnA/off") >= 0){
              Serial.println("BtnA off");
              btnAState = "OFF";
              digitalWrite(LED_A, LOW);
              digitalWrite(OUTPUT_1, LOW);
            } else if (header.indexOf("GET /btnA/on") >= 0){
              Serial.println("BtnA on");
              btnAState = "ON";
              digitalWrite(LED_A, HIGH);
              digitalWrite(OUTPUT_1, HIGH);

            // Btn B sets Output 2
            } else if (header.indexOf("GET /btnB/off") >= 0) {
              Serial.println("BtnB off");
              btnBState = "OFF";
              digitalWrite(LED_B, LOW);
              digitalWrite(OUTPUT_2, LOW);
            } else if (header.indexOf("GET /btnB/on") >= 0) {
              Serial.println("BtnB on");
              btnBState = "ON";
              digitalWrite(LED_B, HIGH);
              digitalWrite(OUTPUT_2, HIGH);

            // Relay Energise 
            } else if (header.indexOf("GET /relay/off") >= 0) {
              Serial.println("Relay off");
              relayState = "OFF";
              digitalWrite(RELAY, LOW);
            } else if (header.indexOf("GET /relay/on") >= 0) {
              Serial.println("Relay on");
              relayState = "ON";
              digitalWrite(RELAY, HIGH);

            } //(header.indexOf()

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            // LED button
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;"); // green LED button
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #F23A3A;}"); // red LED button
            // Button A
            client.println(".button3 {background-color: #4CAF50;}"); // green BtnA button
            client.println(".button4 {background-color: #F23A3A;}"); // red BtnA button
            // Button B
            client.println(".button5 {background-color: #4CAF50;}"); // green BtnB button
            client.println(".button6 {background-color: #F23A3A;}"); // red BtnB button 
            // Relay
            client.println(".button7 {background-color: #4CAF50;}"); // green Relay button
            client.println(".button8 {background-color: #F23A3A;}"); // red Relay button                        

            client.println("</style></head>");

            // Web Page Heading
            client.println("<body><h1>Pico W LED Control</h1>");

            // Display current state, and ON/OFF buttons for Onboard LED
            // Bold ON Normal OFF
            if (picoLEDState == "ON"){
              client.println("<p>Onboard LED is <b>" + picoLEDState + "</b></p>");
            } else {
              client.println("<p>Onboard LED is " + picoLEDState + "</p>");
            }

            if (btnAState == "ON"){
              client.println("<p>Output 1 is <b>" + btnAState + "</b></p>"); 
            } else {
              client.println("<p>Output 1 is " + btnAState + "</p>"); 
            }

            if (btnBState == "ON"){
              client.println("<p>Output 2 is <b>" + btnBState+ "</b></p>"); 
            } else {
              client.println("<p>Output 2 is " + btnBState+ "</p>"); 
            }

            if (relayState == "ON"){
              client.println("<p>Relay    is <b>" + relayState + "</b></p>");  
            } else {
              client.println("<p>Relay    is " + relayState + "</p>");  
            }
                                  
            // Set buttons
            // LED
            if (picoLEDState == "OFF") {
              client.println("<p><a href=\"/led/on\"><button class=\"button\">LED</button></a></p>");
            } else {
              client.println("<p><a href=\"/led/off\"><button class=\"button button2\">LED</button></a></p>");
            }

            // Btn A
            if (btnAState == "OFF"){
              client.println("<p><a href=\"/btnA/on\"><button class=\"button button3\">Btn_A</button></a></p>");
            } else {
              client.println("<p><a href=\"/btnA/off\"><button class=\"button button4\">Btn_A</button></a></p>");
            }

            // Btn B
            if (btnBState == "OFF"){
              client.println("<p><a href=\"/btnB/on\"><button class=\"button button5\">Btn_B</button></a></p>");
            } else {
              client.println("<p><a href=\"/btnB/off\"><button class=\"button button6\">Btn_B</button></a></p>");
            }

            // Relay
            if (relayState == "OFF"){
              client.println("<p><a href=\"/relay/on\"><button class=\"button button7\">Relay</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay/off\"><button class=\"button button8\">Relay</button></a></p>");
            }

            // Display footer information
            client.printf("<p>-----------------------</p>");
            client.printf("<p><b>Temperature is %.2f°C </b></p>", temperature);
            client.printf("<p>(C) Steve Fuller</p>");

            // end HTML code
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          } //(currentLine.length() == 0)

        } else if (c != '\r') {  // if you got anything else but a carriage return character,

          currentLine += c;      // add it to the end of the currentLine

        } // (c == '\n') 

      } //if (client.available())

    } //while (client.connected() && currentTime - previousTime <= timeoutTime)

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

  } //  if (client)

} // loop