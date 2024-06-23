# InfiDig
An SPI based interface library for the [InfiDig](https://github.com/CC-Ian/InfiDig-hardware).

## Installation
Paste the following line in your `platformio.ini` file:
```ini
lib_deps = https://github.com/CC-Ian/InfiDig
```

## Usage
This library is still in beta. It's display function isn't perfect. If you're using an ESP32, consider giving this a dedicated thread as it must be perpetually running to display numbers.

The following is a mix of C++ and pseudocode.

```c++
// Initialize the display manager.
// Clock and data pins optional. Will default to the SPI SCK and MOSI pins if not declared.
DisplayManager dispMgr(LATCH_PIN, CLOCK_PIN, DATA_PIN);

// Show numbers
int numbers[] = {1, 2, 3, 4, ...};
dispMgr.displayNumbers(numbers, numbers.length());
```
