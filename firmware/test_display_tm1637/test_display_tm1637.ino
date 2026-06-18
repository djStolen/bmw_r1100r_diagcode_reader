#include <TM1637Display.h>

// Try using different pins, specifically pins that are not used by anything else
#define CLK 6 
#define DIO 5

TM1637Display display(CLK, DIO);

void setup() {
  // Use a much lower brightness to rule out power-supply noise 
  // causing glitches during transmission
  display.setBrightness(0x01); 
  delay(500);
}

void loop() {
  // Try writing a simple fixed pattern instead of using the number formatter
  uint8_t data[] = { 0b00111111, 0b00111111, 0b00111111, 0b00111111 }; // Should display '8888'
  display.setSegments(data);
  delay(2000);
  
  display.clear(); // Should be completely blank
  delay(2000);
}