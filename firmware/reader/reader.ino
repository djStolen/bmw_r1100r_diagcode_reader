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
  // Initialize TTY / Serial Output
  Serial.begin(115200);
  Serial.println("====================================");
  Serial.println("BMW R1100R Motronic Code Reader v1.0");
  Serial.println("====================================");

  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT_PULLUP);
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
  
  // Log the reset event to the TTY
  Serial.println("\n[SYSTEM] Ready. Waiting for ECU pulses...");
}

void loop() {
  // --- NON-BLOCKING HEARTBEAT ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeatTime >= heartbeatInterval) {
    lastHeartbeatTime = currentMillis;
    ledState = !ledState; 
    digitalWrite(HEARTBEAT_LED, ledState);
  }
  // ------------------------------
  
  // Poll time delta progressions 
  if (decoder.checkTimeout(millis())) {
    int fetchedCode = decoder.getLatestCode();
    
    // Update the physical 7-segment display
    display.showNumberDec(fetchedCode, true);
    
    // Print the formatted code to the TTY
    Serial.print("[DECODED] Fault Code: ");
    
    // Handle edge case where code 0000 might print as just "0"
    if (fetchedCode == 0) {
      Serial.println("0000 (End of Sequence)");
    } else if (fetchedCode == 4444) {
      Serial.println("4444 (No Faults Stored)");
    } else {
      Serial.println(fetchedCode);
    }
  }

  // Handle local user resets
  if (digitalRead(RESET_BTN) == LOW) {
    localReset();
    delay(400); // Debounce physical push button action
  }
}
