#include <displayManager.h>

// CharliePlexing LEDs. These maps set which pins are high and which are low.
// Looking at the schematic, the LSB is labeled A on the 6 point LED array. This counts up counter-clockwise around the display.
// I'm grouping segments into pairs to save on some clock cycles in theory.
// The first pair/column represents the top vertical segments and the bottommost horizontal segment. Segments C-B and D-E.
// The second pair/column represents the bottom vertical segments. Segments B-A and E-F.
// The third pair/column represents the top two horizontal segments or all three horizontal segments. Segments D-C and E-B.
uint8_t numberBits[11][3] = {
// 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF     // Decimal representation
  {0b00001101, 0b00010010, 0b00000100},     // 0
  {0b00001000, 0b00010000, 0b00000000},     // 1
// 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
  {0b00001000, 0b00110010, 0b00000111},     // 2
  {0b00001000, 0b00010000, 0b00000111},     // 3
// 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
  {0b00001100, 0b00010000, 0b00100011},     // 4
  {0b00111100, 0b00010000, 0b00000111},     // 5
// 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
  {0b00111100, 0b00010010, 0b00000111},     // 6
  {0b00001000, 0b00010000, 0b00110111},     // 7
// 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
  {0b00001100, 0b00010010, 0b00000111},     // 8
  {0b00001100, 0b00010000, 0b00100111},     // 9
// 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
  {0b00000000, 0b00000000, 0b00000000}      // [blank]
};
uint8_t signBits[4][3] {
// 0bABCE, 0bABCE, 0bABCE
  {0b0000, 0b0000, 0b0000},   // positive below |100|
  {0b0010, 0b0100, 0b0000},   // positive above |100|
  {0b0000, 0b0000, 0b0001},   // negative below |100|
  {0b0010, 0b0100, 0b0001},   // negative above |100|
};

/// @brief Main Class Initializer for EEAN 2.5 digit displays.
/// @param latchPin The latch pin is used to store data in the shift register once it has been sent. (P12, RCLK)
/// @param clockPin The clock pin is the main clock for the shift register. (P11 SRCLK)
/// @param dataPin The data pin is the pin that data is sent out along. (P14, SER)
DisplayManager::DisplayManager(int8_t latchPin, int8_t clockPin, int8_t dataPin) : _latchPin(latchPin), _clockPin(clockPin), _dataPin(dataPin) {}

/// @brief Initializer. Starts SPI and initializes the latch pin.
void DisplayManager::begin() {
    // If the clock or data pins are undefined, use defaults. Else use user defined pins for clock and data.
    if (_clockPin == -1 || _dataPin == -1) {
        SPI.begin();
    } else {
        #if defined(ESP32) || defined(ESP8266)
            SPI.begin(_clockPin, -1, _dataPin, -1);
        #else
            SPI.begin();
        #endif
    }
    // Set latch as output active high.
    pinMode(_latchPin, OUTPUT);
    digitalWrite(_latchPin, LOW);
}

/// @brief Displays the input number array on the display boards.
/// @param numbers An array of numbers to display in the format {0, 1, 2, 3, ...}
/// @param count An integer length of the numbers array.
void DisplayManager::displayNumbers(int numbers[], int count) {
    // Begin the SPI Transaction using a speed of 5MHz. This is the rated speed at 2.0v on the 75HC595.
    SPI.beginTransaction(SPISettings(5000000, LSBFIRST, SPI_MODE0));
    // Loop through the three "frames" for each number.
    for (size_t frameStep = 0; frameStep < 3; frameStep++) {
        // Loop through each number in the numbers array and send it's 16 bit code to the shift registers.
        for (size_t j = count; j > 0; j--) {
            uint16_t bitwiseRepresentation = prepareTransferBits(numbers[j-1], frameStep);
            SPI.transfer16(bitwiseRepresentation);
        }
        // Pulse the latch to clock in the new data.
        digitalWrite(_latchPin, HIGH);
        digitalWrite(_latchPin, LOW);
        // Wait one millisecond to aid in persistence of vision.
        // Need some time for the LEDs to turn on and emit light.
        delay(1);
    }
    // End the SPI Transaction.
    SPI.endTransaction();
}

/// @brief Convert the input number (-199 to +199) to a 16 bit string suitable for display.
/// @param number The number to convert.
/// @param frameStep The frameStep (0 to 2) to display.
/// @return An unsigned 16 bit integer ready to be displayed.
uint16_t DisplayManager::prepareTransferBits(int number, uint8_t frameStep) {
    // Enforce Limits.
    if (number > 199) {
        number = 199;
    } else if (number < -199) {
        number = -199;
    }

    // Break the input number apart into three digits.
    int onesPlace = abs(number) % 10;
    int tensPlace = (abs(number) / 10) % 10;
    int hundredsPlace = 0;

    // Manually set the hundreds place based on the value of the input number.
    if (number > 99)
        // Show a one.
        hundredsPlace = 1;
    else if (number < 0 && number > -100)
        // Show the negative sign.
        hundredsPlace = 2;
    else if (number < -99)
        // Show a negative 1one.
        hundredsPlace = 3;

    // Remove the tens place if the magnitude of the input number is less than 10.
    if (abs(number) < 10)
        tensPlace = 10;

    // return a formatted number string.
    return(numberBits[onesPlace][frameStep] | (numberBits[tensPlace][frameStep] << 6) | (signBits[hundredsPlace][frameStep] << 12));
}

/// @brief Clears all displays by setting the shift register to all zeros.
/// @param numberOfDisplays The number of displays to clear from the start of the chain.
void DisplayManager::clearDisplays(int numberOfDisplays) {
  for (size_t i = 0; i < numberOfDisplays; i++) {
    SPI.transfer16(0);
    digitalWrite(_latchPin, HIGH);
    digitalWrite(_latchPin, LOW);
  }
}