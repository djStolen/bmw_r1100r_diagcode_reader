#include <Arduino.h>

#define OUTPUT_PIN 4  
#define TRIGGER_PIN 2 

const int TOTAL_CODES = 9;
const int testCodes[TOTAL_CODES][4] = {
  {1, 1, 2, 2}, {1, 1, 3, 3}, {1, 2, 1, 5},
  {1, 2, 2, 3}, {1, 2, 2, 4}, {2, 3, 4, 1},
  {2, 3, 4, 2}, {4, 4, 4, 4}, {0, 0, 0, 0}
};

// Heartbeat variables
const int HEARTBEAT_LED = 13;        // Onboard LED on Arduino Nano
unsigned long lastHeartbeatTime = 0; 
const long heartbeatInterval = 2000;  // Blink every 2000ms
bool ledState = LOW;

void setup() {
  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  digitalWrite(OUTPUT_PIN, HIGH); // Default open collector pullup status 
}

void sendDigit(int count) {
  if (count > 0) {
    for (int i = 0; i < count; i++) {
      digitalWrite(OUTPUT_PIN, LOW);  
      delay(150);                     
      digitalWrite(OUTPUT_PIN, HIGH); 
      delay(150);                     
    }
  }
  delay(1000); // Digit gap identifier
}

void sendCode(const int code[4]) {
  for (int i = 0; i < 4; i++) {
    sendDigit(code[i]);
  }
}

void heartbeat() {
  // --- NON-BLOCKING HEARTBEAT ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeatTime >= heartbeatInterval) {
    lastHeartbeatTime = currentMillis;
    ledState = !ledState; // Toggle the LED state
    digitalWrite(HEARTBEAT_LED, ledState);
  }
  // ------------------------------
}

void loop() {
  heartbeat();

  if (digitalRead(TRIGGER_PIN) == LOW) {
    unsigned long pressStart = millis();
    while (digitalRead(TRIGGER_PIN) == LOW) { delay(10); }
    
    if (millis() - pressStart >= 4500) {
      delay(1000); 
      for (int c = 0; c < TOTAL_CODES; c++) {
        sendCode(testCodes[c]);
        delay(4000); // Code block transmission gap
      }
    }
  }
}

