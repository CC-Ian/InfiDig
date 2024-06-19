#include <Arduino.h>
#include <SPI.h>

class DisplayManager {
public:
    DisplayManager(uint8_t larchPin, uint8_t clockPin = -1, uint8_t dataPin = -1);
    void begin();
    void displayNumbers(int numbers[], int count);
private:
    uint8_t _latchPin;
    uint8_t _clockPin;
    uint8_t _dataPin;
    uint16_t prepareTransferBits(int number, uint8_t frameStep);
};