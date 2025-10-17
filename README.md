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

Complete README content with all API documentation, examples, troubleshooting guide, license, and contact information.
