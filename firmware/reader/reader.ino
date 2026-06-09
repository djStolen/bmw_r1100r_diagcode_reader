#include "src/MotronicDecoder.h"
#include <TM1637Display.h>

#define CLK 6
#define DIO 5
#define SIGNAL_PIN 2  
#define RESET_BTN 3   

TM1637Display display(CLK, DIO);
MotronicDecoder decoder(2500, 150);

// Heartbeat variables
const int HEARTBEAT_LED = 13;        // Onboard LED on Arduino Nano
unsigned long lastHeartbeatTime = 0; 
const long heartbeatInterval = 500;  // Blink every 500ms
bool ledState = LOW;

void setup() {
  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(RESET_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), handleInterrupt, FALLING);
  
  display.setBrightness(0x0f);
  localReset();
}

void handleInterrupt() {
  decoder.registerPulse(millis());
}

void localReset() {
  decoder.reset();
  uint8_t readyPattern[] = { 0x40, 0x40, 0x40, 0x40 }; // ----
  display.setSegments(readyPattern);
}

void loop() {
  // --- NON-BLOCKING HEARTBEAT ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeatTime >= heartbeatInterval) {
    lastHeartbeatTime = currentMillis;
    ledState = !ledState; // Toggle the LED state
    digitalWrite(HEARTBEAT_LED, ledState);
  }
  // ------------------------------
  // Poll time delta progressions 
  if (decoder.checkTimeout(millis())) {
    display.showNumberDec(decoder.getLatestCode(), true);
  }

  // Handle local user resets
  if (digitalRead(RESET_BTN) == LOW) {
    localReset();
    delay(400); // Debounce physical push button action
  }
}

