:warning: This driver (and the self praises :-D) is a raw output from Claude Sonnet AI and it is currently untested.  
It will probably not work as is.

# AS6500 Time-to-Digital Converter Arduino Driver

A comprehensive Arduino library for interfacing with the AS6500 high-precision Time-to-Digital Converter (TDC) via SPI communication. This library provides an easy-to-use API for measuring time intervals with picosecond resolution.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Wiring Diagram](#wiring-diagram)
- [Quick Start Guide](#quick-start-guide)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Advanced Usage](#advanced-usage)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Overview

The AS6500 is a high-precision Time-to-Digital Converter (TDC) capable of measuring time intervals with picosecond resolution. This Arduino library provides a complete interface to the AS6500, handling all low-level SPI communication, calibration, and configuration, allowing you to focus on your time measurement application.

**Key Specifications:**
- Time resolution: Picosecond precision
- Interface: SPI (up to 10 MHz)
- Supply voltage: 3.3V
- Operating modes: Single-shot and continuous
- Built-in calibration: Automatic temperature compensation

## Features

- ✅ **Full SPI Communication** - Complete SPI protocol implementation with configurable clock speeds up to 10 MHz
- ✅ **Multiple Measurement Modes** - Single-shot and continuous measurement modes
- ✅ **Automatic Calibration** - Built-in calibration routines with verification and timeout handling
- ✅ **Flexible Configuration** - Configurable timeout values (0-15) and signal averaging (1, 2, 4, or 8 cycles)
- ✅ **Interrupt Management** - Complete interrupt status monitoring and control
- ✅ **High Precision** - Access to both picosecond and nanosecond time resolution
- ✅ **Error Handling** - Comprehensive timeout and error detection with status reporting
- ✅ **Easy-to-Use API** - High-level convenience functions for common operations
- ✅ **Low-Level Access** - Direct register read/write access for advanced users
- ✅ **Well Documented** - Extensive inline documentation and examples

## Hardware Requirements

- **Arduino Board**: Uno, Mega, Due, or any compatible board with SPI support
- **AS6500 TDC Module**: Time-to-Digital Converter chip
- **Connections**: SPI interface (MOSI, MISO, SCK, CS)
- **Level Shifters** (if needed): For 5V Arduino boards to interface with 3.3V AS6500

## Installation

### Method 1: Arduino Library Manager (Recommended - when published)
1. Open the Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for "AS6500"
4. Click **Install**

### Method 2: Manual Installation
1. Download this repository as a ZIP file
2. In Arduino IDE, go to **Sketch** → **Include Library** → **Add .ZIP Library**
3. Select the downloaded ZIP file
4. Restart the Arduino IDE

### Method 3: Git Clone
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/KajWiik/AS6500_arduino.git
```

## Wiring Diagram

### Standard Arduino (Uno, Nano, etc.)

| AS6500 Pin | Arduino Pin | Description          |
|------------|-------------|----------------------|
| VDD        | 3.3V        | Power supply (3.3V)  |
| GND        | GND         | Ground               |
| CS         | 10          | Chip Select (configurable) |
| MOSI       | 11          | SPI Master Out Slave In |
| MISO       | 12          | SPI Master In Slave Out |
| SCK        | 13          | SPI Clock            |

### Arduino Mega

| AS6500 Pin | Arduino Mega Pin | Description          |
|------------|------------------|----------------------|
| VDD        | 3.3V             | Power supply (3.3V)  |
| GND        | GND              | Ground               |
| CS         | 10 (or any)      | Chip Select          |
| MOSI       | 51               | SPI Master Out       |
| MISO       | 50               | SPI Master In        |
| SCK        | 52               | SPI Clock            |

### Arduino Due

| AS6500 Pin | Arduino Due Pin  | Description          |
|------------|------------------|----------------------|
| VDD        | 3.3V             | Power supply (3.3V)  |
| GND        | GND              | Ground               |
| CS         | 10 (or any)      | Chip Select          |
| MOSI       | ICSP-4           | SPI Master Out       |
| MISO       | ICSP-1           | SPI Master In        |
| SCK        | ICSP-3           | SPI Clock            |

⚠️ **Important:** The AS6500 operates at 3.3V. If using a 5V Arduino (Uno, Nano, etc.), use a level shifter or voltage divider for the MOSI, SCK, and CS lines to protect the AS6500.

## Quick Start Guide

Here's a minimal example to get started with the AS6500:

```cpp
#include <AS6500.h>

#define CS_PIN 10

AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    
    // Initialize the TDC
    if (!tdc.begin()) {
        Serial.println("Failed to initialize AS6500!");
        while (1);
    }
    
    // Perform calibration
    if (!tdc.calibrate()) {
        Serial.println("Calibration failed!");
        while (1);
    }
    
    // Configure and enable
    tdc.setMode(MODE_SINGLE_SHOT);
    tdc.setTimeout(10);
    tdc.enable(true);
    
    Serial.println("AS6500 ready!");
}

void loop() {
    // Start measurement
    tdc.startMeasurement();
    
    // Wait for result
    while (!tdc.isResultReady()) {
        delay(1);
    }
    
    // Read and display result
    float timeNs = tdc.getTimeNanoseconds();
    Serial.print("Time: ");
    Serial.print(timeNs, 3);
    Serial.println(" ns");
    
    delay(500);
}
```

## API Reference

### Constructor and Initialization

#### `AS6500(uint8_t csPin)`
Creates an AS6500 object.
- **Parameters:**
  - `csPin`: Chip Select pin number
- **Example:**
  ```cpp
  AS6500 tdc(10);  // CS on pin 10
  ```

#### `bool begin(uint32_t spiSpeed = 1000000)`
Initializes the AS6500 with SPI communication, performs reset, checks chip ID, and runs initial calibration.
- **Parameters:**
  - `spiSpeed`: SPI clock speed in Hz (default: 1 MHz, max: 10 MHz)
- **Returns:** `true` if initialization successful, `false` otherwise
- **Example:**
  ```cpp
  if (!tdc.begin(1000000)) {
      Serial.println("Initialization failed!");
  }
  ```

### Configuration Methods

#### `void setMode(AS6500_Mode mode)`
Sets the measurement mode.
- **Parameters:**
  - `mode`: `MODE_SINGLE_SHOT` or `MODE_CONTINUOUS`
- **Example:**
  ```cpp
  tdc.setMode(MODE_CONTINUOUS);
  ```

#### `void setTimeout(uint8_t timeout)`
Sets the timeout value for measurements.
- **Parameters:**
  - `timeout`: Timeout value (0-15, higher values = longer timeout)
- **Example:**
  ```cpp
  tdc.setTimeout(10);
  ```

#### `void setAveragingCycles(uint8_t cycles)`
Sets the number of averaging cycles.
- **Parameters:**
  - `cycles`: 0=1 average, 1=2 averages, 2=4 averages, 3=8 averages
- **Example:**
  ```cpp
  tdc.setAveragingCycles(2);  // 4 averages
  ```

### Measurement Methods

#### `void startMeasurement()`
Initiates a time measurement.
- **Example:**
  ```cpp
  tdc.startMeasurement();
  ```

#### `bool isResultReady()`
Checks if a new measurement result is available.
- **Returns:** `true` if new result ready, `false` otherwise
- **Example:**
  ```cpp
  if (tdc.isResultReady()) {
      // Read result
  }
  ```

#### `bool readResult(AS6500_Result* result)`
Reads the measurement result into a structure.
- **Parameters:**
  - `result`: Pointer to AS6500_Result structure
- **Returns:** `true` if read successful, `false` otherwise
- **Example:**
  ```cpp
  AS6500_Result result;
  if (tdc.readResult(&result)) {
      Serial.println(result.timeValue);
  }
  ```

### Utility Methods

#### `void reset()`
Resets the AS6500 device.
- **Example:**
  ```cpp
  tdc.reset();
  ```

#### `void enable(bool enable = true)`
Enables or disables the TDC.
- **Parameters:**
  - `enable`: `true` to enable, `false` to disable
- **Example:**
  ```cpp
  tdc.enable(true);
  ```

#### `bool calibrate()`
Performs device calibration.
- **Returns:** `true` if calibration successful, `false` on timeout
- **Example:**
  ```cpp
  if (tdc.calibrate()) {
      Serial.println("Calibration successful");
  }
  ```

### Result Reading Methods

#### `uint32_t getTimePicoseconds()`
Gets the time measurement in picoseconds.
- **Returns:** Time value in picoseconds, or 0 if invalid
- **Example:**
  ```cpp
  uint32_t timePs = tdc.getTimePicoseconds();
  ```

#### `float getTimeNanoseconds()`
Gets the time measurement in nanoseconds.
- **Returns:** Time value in nanoseconds, or 0 if invalid
- **Example:**
  ```cpp
  float timeNs = tdc.getTimeNanoseconds();
  ```

### Interrupt Methods

#### `uint8_t getInterruptStatus()`
Reads the interrupt status register.
- **Returns:** Interrupt status byte with flags:
  - `AS6500_INT_NEW_RESULT`: New result available
  - `AS6500_INT_TIMEOUT`: Measurement timeout
  - `AS6500_INT_CALIBRATION_DONE`: Calibration complete
  - `AS6500_INT_ERROR`: Error occurred
- **Example:**
  ```cpp
  uint8_t status = tdc.getInterruptStatus();
  if (status & AS6500_INT_ERROR) {
      Serial.println("Error!");
  }
  ```

#### `void clearInterrupts(uint8_t flags = 0xFF)`
Clears interrupt flags.
- **Parameters:**
  - `flags`: Flags to clear (default: all flags)
- **Example:**
  ```cpp
  tdc.clearInterrupts(AS6500_INT_NEW_RESULT);
  ```

#### `void setInterruptMask(uint8_t mask)`
Sets the interrupt mask register.
- **Parameters:**
  - `mask`: Interrupt mask byte
- **Example:**
  ```cpp
  tdc.setInterruptMask(0x01);
  ```

### Low-Level Register Access

#### `void writeRegister(uint8_t reg, uint8_t value)`
Writes a value to a register.
- **Parameters:**
  - `reg`: Register address
  - `value`: Value to write
- **Example:**
  ```cpp
  tdc.writeRegister(AS6500_REG_CONFIG0, 0x01);
  ```

#### `uint8_t readRegister(uint8_t reg)`
Reads a value from a register.
- **Parameters:**
  - `reg`: Register address
- **Returns:** Register value
- **Example:**
  ```cpp
  uint8_t value = tdc.readRegister(AS6500_REG_CONFIG0);
  ```

#### `void readMultipleRegisters(uint8_t reg, uint8_t* buffer, uint8_t length)`
Reads multiple consecutive registers.
- **Parameters:**
  - `reg`: Starting register address
  - `buffer`: Buffer to store data
  - `length`: Number of bytes to read
- **Example:**
  ```cpp
  uint8_t data[4];
  tdc.readMultipleRegisters(AS6500_REG_TIME_RESULT_0, data, 4);
  ```

### Other Methods

#### `uint8_t getChipID()`
Reads the chip ID.
- **Returns:** Chip ID value (should be 0x65 for AS6500)
- **Example:**
  ```cpp
  uint8_t id = tdc.getChipID();
  ```

#### `uint16_t getCalibrationValue()`
Gets the last calibration result value.
- **Returns:** Calibration value
- **Example:**
  ```cpp
  uint16_t calVal = tdc.getCalibrationValue();
  ```

## Examples

### Basic Measurement

This example shows how to take single-shot measurements:

```cpp
#include <AS6500.h>

#define CS_PIN 10
AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    
    if (!tdc.begin()) {
        Serial.println("Init failed!");
        while (1);
    }
    
    tdc.setMode(MODE_SINGLE_SHOT);
    tdc.setTimeout(10);
    tdc.enable(true);
}

void loop() {
    tdc.startMeasurement();
    
    while (!tdc.isResultReady()) {
        delay(1);
    }
    
    AS6500_Result result;
    if (tdc.readResult(&result)) {
        Serial.print("Time: ");
        Serial.print(result.timeValue);
        Serial.println(" ps");
    }
    
    delay(500);
}
```

### Continuous Mode

This example demonstrates continuous measurements:

```cpp
#include <AS6500.h>

#define CS_PIN 10
AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    
    if (!tdc.begin()) {
        Serial.println("Init failed!");
        while (1);
    }
    
    // Configure for continuous mode
    tdc.setMode(MODE_CONTINUOUS);
    tdc.setTimeout(8);
    tdc.setAveragingCycles(1);  // 2 averages
    tdc.enable(true);
}

void loop() {
    if (tdc.isResultReady()) {
        float timeNs = tdc.getTimeNanoseconds();
        Serial.print("Time: ");
        Serial.print(timeNs, 3);
        Serial.println(" ns");
    }
    
    delay(10);
}
```

### Interrupt-Driven Measurement

This example shows how to monitor interrupts:

```cpp
#include <AS6500.h>

#define CS_PIN 10
AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    
    if (!tdc.begin()) {
        Serial.println("Init failed!");
        while (1);
    }
    
    tdc.setMode(MODE_SINGLE_SHOT);
    tdc.enable(true);
}

void loop() {
    tdc.startMeasurement();
    
    // Poll interrupt status
    uint8_t status = tdc.getInterruptStatus();
    
    if (status & AS6500_INT_NEW_RESULT) {
        float timeNs = tdc.getTimeNanoseconds();
        Serial.print("Time: ");
        Serial.print(timeNs, 3);
        Serial.println(" ns");
    }
    
    if (status & AS6500_INT_TIMEOUT) {
        Serial.println("Timeout!");
        tdc.clearInterrupts(AS6500_INT_TIMEOUT);
    }
    
    if (status & AS6500_INT_ERROR) {
        Serial.println("Error!");
        tdc.clearInterrupts(AS6500_INT_ERROR);
    }
    
    delay(500);
}
```

## Advanced Usage

### Custom Calibration

For applications requiring periodic recalibration (e.g., due to temperature changes):

```cpp
void performPeriodicCalibration() {
    static unsigned long lastCal = 0;
    
    if (millis() - lastCal > 60000) {  // Every 60 seconds
        Serial.println("Recalibrating...");
        
        if (tdc.calibrate()) {
            uint16_t calValue = tdc.getCalibrationValue();
            Serial.print("New calibration value: ");
            Serial.println(calValue);
            lastCal = millis();
        } else {
            Serial.println("Calibration failed!");
        }
    }
}
```

### Direct Register Access

For advanced users who need to access registers not exposed by the API:

```cpp
// Read a custom register
uint8_t customValue = tdc.readRegister(0x15);

// Write to a custom register
tdc.writeRegister(0x15, 0x42);

// Read multiple registers at once
uint8_t buffer[8];
tdc.readMultipleRegisters(0x10, buffer, 8);
```

### Interrupt Handling with External Pin

If you connect an interrupt pin from the AS6500 to an Arduino pin:

```cpp
#define INT_PIN 2
volatile bool measurementReady = false;

void setup() {
    pinMode(INT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), onMeasurement, FALLING);
    
    // Initialize TDC
    tdc.begin();
    tdc.setInterruptMask(AS6500_INT_NEW_RESULT);
}

void onMeasurement() {
    measurementReady = true;
}

void loop() {
    if (measurementReady) {
        measurementReady = false;
        float timeNs = tdc.getTimeNanoseconds();
        // Process measurement
    }
}
```

### Multiple Measurements with Statistics

Taking multiple measurements and calculating statistics:

```cpp
void takeMeasurements(int count) {
    uint32_t sum = 0;
    uint32_t min_val = 0xFFFFFFFF;
    uint32_t max_val = 0;
    
    for (int i = 0; i < count; i++) {
        tdc.startMeasurement();
        
        while (!tdc.isResultReady()) {
            delay(1);
        }
        
        uint32_t timePs = tdc.getTimePicoseconds();
        sum += timePs;
        
        if (timePs < min_val) min_val = timePs;
        if (timePs > max_val) max_val = timePs;
    }
    
    uint32_t average = sum / count;
    
    Serial.print("Average: ");
    Serial.print(average);
    Serial.println(" ps");
    Serial.print("Min: ");
    Serial.print(min_val);
    Serial.println(" ps");
    Serial.print("Max: ");
    Serial.print(max_val);
    Serial.println(" ps");
}
```

## Troubleshooting

### Initialization Fails

**Problem:** `begin()` returns `false`

**Solutions:**
- Verify wiring connections (especially CS, MOSI, MISO, SCK)
- Check power supply is 3.3V
- Ensure SPI pins are correct for your Arduino board
- Add a small delay before calling `begin()`: `delay(100);`
- Check that AS6500 is properly powered and not damaged

### Chip ID Mismatch

**Problem:** Chip ID is not 0x65

**Solutions:**
- Verify SPI connections
- Check SPI clock speed (try lowering to 100kHz): `tdc.begin(100000);`
- Ensure level shifters (if used) are bidirectional for MISO line
- Try adding pull-up resistors to SPI lines

### Calibration Fails

**Problem:** `calibrate()` returns `false`

**Solutions:**
- Ensure device is enabled: `tdc.enable(true);`
- Increase calibration timeout in AS6500.cpp (currently 100ms)
- Check that the device is not in continuous measurement mode during calibration
- Verify power supply stability (calibration is sensitive to power fluctuations)

### No Measurement Results

**Problem:** `isResultReady()` never returns `true`

**Solutions:**
- Ensure device is enabled: `tdc.enable(true);`
- Check measurement mode is set correctly
- Verify timeout value is not too short
- Check interrupt status for errors: `tdc.getInterruptStatus()`
- Ensure START/STOP signals are being provided to the AS6500 (hardware connection)

### Timeout Errors

**Problem:** Frequent `AS6500_INT_TIMEOUT` interrupts

**Solutions:**
- Increase timeout value: `tdc.setTimeout(15);`
- Verify START/STOP signal timing meets AS6500 specifications
- Check signal quality and noise on input lines
- Ensure proper grounding

### Incorrect Measurements

**Problem:** Time measurements seem wrong or inconsistent

**Solutions:**
- Perform calibration: `tdc.calibrate();`
- Increase averaging cycles: `tdc.setAveragingCycles(3);` (8 averages)
- Check for noise on input signals
- Verify power supply is stable and clean
- Ensure proper PCB layout (short traces, good grounding)

### SPI Communication Errors

**Problem:** Random read/write errors

**Solutions:**
- Reduce SPI clock speed: `tdc.begin(500000);`
- Check wire length (keep SPI traces/wires short)
- Add decoupling capacitors near AS6500 (0.1µF)
- Ensure CS line is stable and not floating
- Try adding small delays between operations

## Contributing

We welcome contributions to the AS6500 Arduino library! Here's how you can help:

### Reporting Issues

- Use GitHub Issues to report bugs
- Include your Arduino board model and IDE version
- Provide a minimal code example that demonstrates the issue
- Include any error messages or unexpected behavior details

### Submitting Pull Requests

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-feature`
3. Make your changes with clear, descriptive commits
4. Test your changes on real hardware if possible
5. Update documentation as needed
6. Submit a pull request with a clear description of changes

### Code Style Guidelines

- Follow the existing code style
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and concise
- Update examples if API changes

### Testing

- Test on multiple Arduino platforms when possible (Uno, Mega, Due)
- Verify SPI communication at different clock speeds
- Ensure backward compatibility unless making breaking changes

### Documentation

- Update README.md for new features
- Add inline code comments for complex sections
- Create examples for new functionality
- Update API reference for new methods

## License

MIT License

Copyright (c) 2025 KajWiik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
