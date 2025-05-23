#include <Arduino.h>
#include "button.h"

// Use digital pin 2 for the button.Button between pin 2 and GND
Button myButton(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  buttonevent ev = myButton.checkButton();
    if (ev & B_PRESS) {
      Serial.println("Button pressed");
    }
    if (ev & (B_HOLD | B_PRESS)) {
      Serial.print("Button held (repeat)");
    }  
    if ((ev & B_TIMEOUT) && ((ev & B_COUNT) == 1)) {
      Serial.println("Single click detected");
    }
    if ((ev & B_TIMEOUT) && ((ev & B_COUNT) == 2)) {
      Serial.println("Double click detected");
    }
    // Add more event handling as needed
}