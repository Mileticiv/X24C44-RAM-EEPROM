#ifndef X24C44_H
#define X24C44_H

#include <Arduino.h>

class X24C44 {
private:
    byte CE, SK, DI, DO, recal, store;

    void pin(bool value);
    void sendInstruction(byte instruction);
    
public:
    X24C44(byte ce, byte sk, byte di, byte doPin, byte recalPin, byte storePin);

    void begin();
    void writeByte(byte address, word data);
    word readByte(byte address);
    void RAMtoEEPROM();
    void DisablesWritesAndStores();
};

#endif
