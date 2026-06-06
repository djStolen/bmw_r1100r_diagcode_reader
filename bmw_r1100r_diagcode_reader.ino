#include <TM1637Display.h>

#define CLK 6
#define DIO 5
#define SIGNAL_PIN 2  // Fault code input
#define RESET_BTN 3   // Local reset button

TM1637Display display(CLK, DIO);

volatile int pulseCount = 0;
unsigned long lastPulseTime = 0;
int digits[4] = {0, 0, 0, 0};
int currentDigit = 0;
const unsigned long gapThreshold = 2500; // 2.5s gap means next digit

void setup() {
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(RESET_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), countPulse, FALLING);
  
  display.setBrightness(0x0f);
  clearSystem();
}

void countPulse() {
  unsigned long now = millis();
  if (now - lastPulseTime > 150) { // Motronic pulses are chunky (~200ms)
    pulseCount++;
    lastPulseTime = now;
  }
}

void clearSystem() {
  pulseCount = 0;
  currentDigit = 0;
  for(int i=0; i<4; i++) digits[i] = 0;
  uint8_t ready[] = { 0x40, 0x40, 0x40, 0x40 }; // ----
  display.setSegments(ready);
  delay(1000);
  display.showNumberDec(0, true);
}

void loop() {
  // 1. Check for Reset
  if (digitalRead(RESET_BTN) == LOW) {
    clearSystem();
    delay(500);
  }

  // 2. Logic to catch the end of a digit pulse train
  if (pulseCount > 0 && (millis() - lastPulseTime > gapThreshold)) {
    if (currentDigit < 4) {
      digits[currentDigit] = pulseCount;
      currentDigit++;
      pulseCount = 0;
      
      // Update display with current progress
      int code = (digits[0] * 1000) + (digits[1] * 100) + (digits[2] * 10) + digits[3];
      display.showNumberDec(code, true);
    }
  }
}