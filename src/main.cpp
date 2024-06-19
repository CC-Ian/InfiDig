#include <Arduino.h>
#include <SPI.h>
#include <displayManager.h>

#define CLK 18
#define DATA_IN 23
#define LATCH 2
// #define OUTPUT_ENABLE

// // CharliePlexing LEDs. These maps set which pins are high and which are low.
// // Looking at the schematic, the LSB is labeled A on the 6 point LED array. This counts up counter-clockwise around the display.
// // I'm grouping segments into pairs to save on some clock cycles in theory.
// // The first pair/column represents the top vertical segments and the bottommost horizontal segment. Segments C-B and D-E.
// // The second pair/column represents the bottom vertical segments. Segments B-A and E-F.
// // The third pair/column represents the top two horizontal segments or all three horizontal segments. Segments D-C and E-B.
// uint8_t numberBits[11][3] = {
// // 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
//   {0b00001101, 0b00010010, 0b00000100},     // 0
//   {0b00001000, 0b00010000, 0b00000000},     // 1
// // 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF 
//   {0b00001000, 0b00110010, 0b00000111},     // 2
//   {0b00001000, 0b00010000, 0b00000111},     // 3
// // 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
//   {0b00001100, 0b00010000, 0b00100011},     // 4
//   {0b00111100, 0b00010000, 0b00000111},     // 5
// // 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
//   {0b00111100, 0b00010010, 0b00000111},     // 6
//   {0b00001000, 0b00010000, 0b00110111},     // 7
// // 0bxxABCDEF, 0bxxABCDEF, 0bxxABCDEF
//   {0b00001100, 0b00010010, 0b00000111},     // 8
//   {0b00001100, 0b00010000, 0b00100111},     // 9
//   {0b00000000, 0b00000000, 0b00000000}      // 10
// };


// uint8_t signBits[4][3] {
// // 0bABCE
//   {0b0000, 0b0000, 0b0000},   // positive below |100|
//   {0b0010, 0b0100, 0b0000},   // positive above |100|
//   {0b0000, 0b0000, 0b0001},   // negative below |100|
//   {0b0010, 0b0100, 0b0001},   // negative above |100|
// };


// /// @brief Displays a number on the segment display for a single cycle of SPI.
// /// @param number The decimal number to display.
// void displayNumber(int number) {
//   SPI.beginTransaction(SPISettings(5000000, LSBFIRST, SPI_MODE0));
//   for (size_t i = 0; i < 3; i++) {
//     SPI.transfer(numberBits[number][i]);
//     digitalWrite(LATCH, 1);
//     digitalWrite(LATCH, 0);
//     delay(1);
//   }
//   SPI.endTransaction();
// }

// void displayBigNumber(int number) {
//   // Initial conversion of number to individual parts.
//   int num1 = abs(number) % 10;
//   int num2 = (abs(number) / 10) % 10;
//   int num3 = 0;

//   // Determine sign bits.
//   if (number > 99) {
//     num3 = 1;
//   } else if (number < 0 && number > -100) {
//     num3 = 2;
//   } else if (number < -99) {
//     num3 = 3;
//   } else {
//     num3 = 0;
//   }

//   // Disable middle segment if the |number| is less than 10.
//   if (abs(number) < 10) {
//     num2 = 10;
//   }
  
//   // Create actual control bits.
//   uint16_t transferBits[3];
//   for (int i = 0; i < 3; i++) {
//     transferBits[i] = numberBits[num1][i] | numberBits[num2][i] << 6 | signBits[num3][i] << 12;
//   }

//   SPI.beginTransaction(SPISettings(5000000, LSBFIRST, SPI_MODE0));
//   for (size_t i = 0; i < 3; i++) {
//     SPI.transfer16(transferBits[i]);
//     digitalWrite(LATCH, 1);
//     digitalWrite(LATCH, 0);
//     delay(1);
//   }
//   SPI.endTransaction();
// }

DisplayManager displayManager(LATCH);

void setup() {
  displayManager.begin();
}

void loop() {
  int numbers[] = {80, 90}; 
  displayManager.displayNumbers(numbers, sizeof(numbers) / sizeof(numbers[0]));
}
