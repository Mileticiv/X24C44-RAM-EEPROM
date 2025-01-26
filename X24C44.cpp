#include "X24C44.h"

X24C44::X24C44(byte ce, byte sk, byte di, byte doPin, byte recalPin, byte storePin) 
    : CE(ce), SK(sk), DI(di), DO(doPin), recal(recalPin), store(storePin) {}

void X24C44::begin() {
    pinMode(recal, OUTPUT);
    digitalWrite(recal, HIGH);

    pinMode(store, OUTPUT);
    digitalWrite(store, HIGH);

    pinMode(CE, OUTPUT);
    digitalWrite(CE, HIGH); // Chip is not selected

    pinMode(SK, OUTPUT);
    digitalWrite(SK, LOW);

    pinMode(DI, OUTPUT);
    digitalWrite(DI, LOW);

    pinMode(DO, INPUT);

    sendInstruction(0b10000101); // RCL instrukcija		Recall EEPROM Data into RAM
    sendInstruction(0b10000100); // WREN instrukcija		Set Write Enable Latch (Enables Writes and Stores)
}

void X24C44::pin(bool value) {
    digitalWrite(DI, value ? HIGH : LOW);
    digitalWrite(SK, HIGH);
    digitalWrite(SK, LOW);
}

void X24C44::sendInstruction(byte instruction) {
    digitalWrite(CE, HIGH);  // Početak instrukcije
    for (int i = 7; i >= 0; i--) {
        pin((instruction >> i) & 0x01);
    }
    digitalWrite(CE, LOW); // Završetak instrukcije
}

void X24C44::writeByte(byte address, word data) {
    digitalWrite(CE, HIGH);
    byte instruction = 0b10000011 | (address << 3); // Slanje WRITE instrukcije (1AAAA011)
    for (int i = 7; i >= 0; i--) {
        pin((instruction >> i) & 0x01);
    }
    for (int i = 15; i >= 0; i--) { // Slanje podataka (16 bitova)
        pin((data >> i) & 0x01);
    }
    digitalWrite(CE, LOW);
}

word X24C44::readByte(byte address) {
    word data = 0;
    digitalWrite(CE, HIGH);
    byte instruction = 0b10000110 | (address << 3); // Slanje READ instrukcije (1AAAA11X)
    for (int i = 7; i >= 0; i--) {
        pin((instruction >> i) & 0x01);
    }
    for (int i = 0; i < 15; i++) { // Čitanje podataka (16 bitova)
        digitalWrite(SK, HIGH);
        data = (data << 1) | digitalRead(DO);
        digitalWrite(SK, LOW);
    }
    digitalWrite(CE, LOW);
    return data;
}

void X24C44::RAMtoEEPROM() {
    sendInstruction(0b10000001); // Store RAM Data in E2PROM
}

void X24C44::DisablesWritesAndStores() {
    sendInstruction(0b10000000); // Reset Write Enable Latch (Disables Writes and Stores)
}