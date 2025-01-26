#include <X24C44.h>

// you may use any other Arduino pin
#define CE 5                  // CE - 1|-------|8 - VCC 5V
#define SK 6                  // SK - 2|*      |7 - STORE
#define DI 7                  // DI - 3|       |6 - RECALL
#define DO 8                  // DO - 4|-------|5 - GND
#define recal 12        // Hardwer control (not in use in this program) connect to VCC
#define store 13        // Hardwer control (not in use in this program) connect to VCC

void setup() {
  Serial.begin(9600);
  eeprom.begin();
  Serial.println("Value read from RAM address x0-9 ");
  for (int x = 0; x < 10; x++) {
    eeprom.writeByte(x, x + 5);            
    word value = eeprom.readByte(x);

    Serial.print(x);
    Serial.print(" - ");
    Serial.println(value);
  }
}

void loop() {
  delay(5000);
}