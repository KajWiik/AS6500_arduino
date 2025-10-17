/**
 * AS6500 Time-to-Digital Converter Arduino Driver
 * 
 * This library provides an interface to the AS6500 TDC chip via SPI
 * 
 * Author: Arduino Driver Implementation
 * Date: 2025-10-17
 */

#ifndef AS6500_H
#define AS6500_H

#include <Arduino.h>
#include <SPI.h>

// AS6500 Register Addresses
#define AS6500_REG_CONFIG0          0x00
#define AS6500_REG_CONFIG1          0x01
#define AS6500_REG_CONFIG2          0x02
#define AS6500_REG_INT_STATUS       0x03
#define AS6500_REG_INT_MASK         0x04
#define AS6500_REG_FIRE_NUM         0x05
#define AS6500_REG_TIME_RESULT_0    0x10
#define AS6500_REG_TIME_RESULT_1    0x11
#define AS6500_REG_TIME_RESULT_2    0x12
#define AS6500_REG_TIME_RESULT_3    0x13
#define AS6500_REG_CALIBRATION      0x20
#define AS6500_REG_CALIBRATION_RES  0x21
#define AS6500_REG_CHIP_ID          0x30

// SPI Commands
#define AS6500_CMD_WRITE            0x00
#define AS6500_CMD_READ             0x80

// Config0 Register Bits
#define AS6500_CONFIG0_ENABLE       0x01
#define AS6500_CONFIG0_RESET        0x02
#define AS6500_CONFIG0_CALIBRATE    0x04
#define AS6500_CONFIG0_MODE_MASK    0x18
#define AS6500_CONFIG0_MODE_1       0x00
#define AS6500_CONFIG0_MODE_2       0x08

// Config1 Register Bits
#define AS6500_CONFIG1_TIMEOUT_MASK 0x0F
#define AS6500_CONFIG1_AVG_CYCLES   0x30

// Interrupt Status Bits
#define AS6500_INT_NEW_RESULT       0x01
#define AS6500_INT_TIMEOUT          0x02
#define AS6500_INT_CALIBRATION_DONE 0x04
#define AS6500_INT_ERROR            0x08

// Timing Constants
#define AS6500_TIMEOUT_MS           1000
#define AS6500_CALIBRATION_TIME_MS  100
#define AS6500_CHIP_ID_VALUE        0x65  // Expected chip ID

// Measurement modes
enum AS6500_Mode {
    MODE_SINGLE_SHOT = 0,
    MODE_CONTINUOUS = 1
};

// Result structure
struct AS6500_Result {
    uint32_t timeValue;     // Time measurement in picoseconds
    bool valid;             // Result validity flag
    uint8_t channelNum;     // Channel number (if applicable)
};

class AS6500 {
public:
    /**
     * Constructor
     * @param csPin Chip Select pin number
     */
    AS6500(uint8_t csPin);
    
    /**
     * Initialize the AS6500
     * @param spiSpeed SPI clock speed (default 1MHz)
     * @return true if initialization successful
     */
    bool begin(uint32_t spiSpeed = 1000000);
    
    /**
     * Reset the device
     */
    void reset();
    
    /**
     * Enable/disable the TDC
     * @param enable true to enable, false to disable
     */
    void enable(bool enable = true);
    
    /**
     * Perform calibration
     * @return true if calibration successful
     */
    bool calibrate();
    
    /**
     * Set measurement mode
     * @param mode Measurement mode (single shot or continuous)
     */
    void setMode(AS6500_Mode mode);
    
    /**
     * Set timeout value for measurements
     * @param timeout Timeout value (0-15)
     */
    void setTimeout(uint8_t timeout);
    
    /**
     * Set number of averaging cycles
     * @param cycles Number of cycles (0-3: 1, 2, 4, 8 averages)
     */
    void setAveragingCycles(uint8_t cycles);
    
    /**
     * Start a measurement
     */
    void startMeasurement();
    
    /**
     * Check if new result is available
     * @return true if new result ready
     */
    bool isResultReady();
    
    /**
     * Read the measurement result
     * @param result Pointer to result structure
     * @return true if read successful
     */
    bool readResult(AS6500_Result* result);
    
    /**
     * Get time measurement in picoseconds
     * @return Time value in picoseconds, or 0 if invalid
     */
    uint32_t getTimePicoseconds();
    
    /**
     * Get time measurement in nanoseconds
     * @return Time value in nanoseconds, or 0 if invalid
     */
    float getTimeNanoseconds();
    
    /**
     * Read interrupt status
     * @return Interrupt status byte
     */
    uint8_t getInterruptStatus();
    
    /**
     * Clear interrupt flags
     * @param flags Flags to clear
     */
    void clearInterrupts(uint8_t flags = 0xFF);
    
    /**
     * Set interrupt mask
     * @param mask Interrupt mask byte
     */
    void setInterruptMask(uint8_t mask);
    
    /**
     * Read chip ID
     * @return Chip ID value
     */
    uint8_t getChipID();
    
    /**
     * Get calibration result
     * @return Calibration value
     */
    uint16_t getCalibrationValue();
    
    /**
     * Write to a register
     * @param reg Register address
     * @param value Value to write
     */
    void writeRegister(uint8_t reg, uint8_t value);
    
    /**
     * Read from a register
     * @param reg Register address
     * @return Register value
     */
    uint8_t readRegister(uint8_t reg);
    
    /**
     * Read multiple registers
     * @param reg Starting register address
     * @param buffer Buffer to store data
     * @param length Number of bytes to read
     */
    void readMultipleRegisters(uint8_t reg, uint8_t* buffer, uint8_t length);

private:
    uint8_t _csPin;
    SPISettings _spiSettings;
    bool _initialized;
    uint16_t _calibrationValue;
    
    void _beginTransaction();
    void _endTransaction();
};

#endif // AS6500_H
