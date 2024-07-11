/* InfiDig example code.
 * This code is built around the ESP32Dev Module (ESP32-Wroom-32E). You can assign SPI clock and data pins or use the defaults.
 * It now runs on an arduino uno without issue. Must use dedicated SPI pins. 13 for clock and 11 for data.
 */


#include <Arduino.h>
#include <displayManager.h>

// // Display Manager object for the filament display. Uses GPIO2 as the latch pin. Default SPI CLOCK and MISO pins.
DisplayManager dispMgr(2);

// Numbers to display.
int numbers[] = {-69};

/// @brief Setup funcition. Runs once on MCU Boot.
void setup() {
  Serial.begin(9600);
  // Begin the display manager.
  dispMgr.begin();

  // Clear out the shift registers. Just sends 0x0000
  dispMgr.clearDisplays(1);
}

/// @brief Loops continuously. Functionally a while(1) with more overhead.
void loop() {
  // MUST perpetually loop to display the numbers.
  // Recommended to pin this to an ESP32 core with xTaskCreatePinnedToCore
  dispMgr.displayNumbers(numbers, 1);
}
