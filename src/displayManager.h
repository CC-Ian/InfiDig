#include <Arduino.h>
#include <SPI.h>

class DisplayManager {
public:
    DisplayManager(int8_t latchPin, int8_t clockPin = -1, int8_t dataPin = -1);
    void begin();
    void displayNumbers(int numbers[], int count);
    void clearDisplays(int numberOfDisplays);
private:
    int8_t _latchPin;
    int8_t _clockPin;
    int8_t _dataPin;
    uint16_t prepareTransferBits(int number, uint8_t frameStep);
};
