#pragma once

#include "globals.h"

namespace wifiSetup{

  void connect(){

    // Connect to Wi-Fi network with SSID and password
    WiFi.begin(ssid, password);

    // Display progress on Serial monitor and flash WiFi LED
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      digitalWrite(LED_WIFI, !digitalRead(LED_WIFI));
      Serial.print(".");
    }

    // Print local IP address and start web server
    Serial.println("");
    Serial.print("WiFi connected at IP Address ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_WIFI, HIGH);

  }

}