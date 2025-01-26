#include <X24C44.h>

// you may use any other Arduino pin
#define CE 5                  // CE - 1|-------|8 - VCC 5V
#define SK 6                  // SK - 2|*      |7 - STORE
#define DI 7                  // DI - 3|       |6 - RECALL
#define DO 8                  // DO - 4|-------|5 - GND
#define recal 12        // Hardwer control (not in use in this program) connect to VCC
#define store 13        // Hardwer control (not in use in this program) connect to VCC

X24C44 eeprom(CE, SK, DI, DO, recal, store);
int adress, DataToSave, ReadValue;
char buffer[100];  // baffer
byte selector;

void setup() {
  Serial.begin(9600);
  eeprom.begin();
  Serial.println("ENTER: *0-READ or 1-SAVE* . ADRESS . DATA   (EXAMPLE SAVE: 1.5.66    (SAVE IN ADRESS 5 DATA 66))");
  Serial.println("ENTER: *2-RAM to EEPROM SAVE");
  Serial.println("ENTER: *3-Disables Writes and Stores in RAM and EEPROM");
  Serial.println("");
  Serial.println("ADRESS - DATA TO SAVE - RAED VALUE - ?");
}

void loop() {
  if (Serial.available() > 0) {
    if (Serial.peek() == '\n' || Serial.peek() == '\r') Serial.read();  // read and discard LF & CR
    else {
      selector = Serial.parseInt();
      adress = Serial.parseInt();
      DataToSave = Serial.parseInt();

      // READ
      if (selector == 0) {
        ReadValue = eeprom.readByte(adress);
        snprintf(buffer, sizeof(buffer), "%-6d - x            - %-10d - ", adress, ReadValue);
        Serial.println(buffer);
      }

      // RAM SAVE & READ
      if (selector == 1) {
        eeprom.writeByte(adress, DataToSave);  // Upis vrednosti X u RAM adresu Y
        ReadValue = eeprom.readByte(adress);
        snprintf(buffer, sizeof(buffer), "%-6d - %-12d - %-10d - ", adress, DataToSave, ReadValue);
        Serial.print(buffer);
        // Compare data
        if (DataToSave == ReadValue) Serial.println("OK");
        else Serial.println("FALSE");
      }

      // SAVE FROM RAM TO EEPROM
      if (selector == 2) {
        eeprom.RAMtoEEPROM();
        tone(2, 1000, 300);
      }

      // Reset Write Enable Latch (Disables Writes and Stores)
      if (selector == 3) {
        eeprom.DisablesWritesAndStores();
        tone(2, 1000, 300);
      }
       // TONE TEST
      if (selector == 4) {
        tone(2, 1000, 300);	// Spiker on pin 2
      }
    }
  }
  delay(250);
}