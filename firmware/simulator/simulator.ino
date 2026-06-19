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
  Serial.begin(115200);
  Serial.println("====================================");
  Serial.println("BMW R1100R Motronic Simulator v1.1");
  Serial.println("====================================");
  Serial.println("[SYSTEM] Ready. Waiting for 5s trigger...");

  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  digitalWrite(OUTPUT_PIN, HIGH); 
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
  delay(1000); // Digit gap
}

void sendCode(const int code[4]) {
  for (int i = 0; i < 4; i++) {
    sendDigit(code[i]);
  }
}

void heartbeat() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeatTime >= heartbeatInterval) {
    lastHeartbeatTime = currentMillis;
    ledState = !ledState;
    digitalWrite(HEARTBEAT_LED, ledState);
  }
}

void loop() {
  heartbeat();

  if (digitalRead(TRIGGER_PIN) == LOW) {
    unsigned long pressStart = millis();
    unsigned long lastLowTime = millis();
    
    while (millis() - lastLowTime < 200) {
      if (digitalRead(TRIGGER_PIN) == LOW) {
        lastLowTime = millis();
      }
      heartbeat(); 
      delay(10);
    }
    
    if (lastLowTime - pressStart >= 4500) {
      Serial.println("\n[TRIGGER] 5s hold detected. Starting sequence...");
      delay(1000); 
      
      for (int c = 0; c < TOTAL_CODES; c++) {
        Serial.print("[TRANSMITTING] Code: ");
        Serial.print(testCodes[c][0]);
        Serial.print(testCodes[c][1]);
        Serial.print(testCodes[c][2]);
        Serial.println(testCodes[c][3]);
        
        sendCode(testCodes[c]);
        delay(4000); 
      }
      Serial.println("[SYSTEM] Sequence complete. Standby.");
    } else {
      Serial.println("[WARNING] Trigger too short. Hold 5s.");
    }
  }
}