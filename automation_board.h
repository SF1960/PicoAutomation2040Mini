# pragma once

namespace boardSetup{

  void setupPins(){

    // Initialize the outputs as an output
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_WIFI, OUTPUT);
    pinMode(LED_A, OUTPUT);
    pinMode(LED_B, OUTPUT);

    pinMode(RELAY,OUTPUT);

    pinMode(OUTPUT_1, OUTPUT);
    pinMode(OUTPUT_2, OUTPUT);

  }

  void initPins(){

    // Initialise the inputs
    pinMode(SWITCH_A, INPUT_PULLUP);
    pinMode(SWITCH_B, INPUT_PULLUP); 

    // Set outputs off
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_WIFI, LOW);
    digitalWrite(LED_A, LOW);
    digitalWrite(LED_B, LOW);
    digitalWrite(RELAY, LOW);
      
  }

}
