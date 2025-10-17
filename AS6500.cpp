/**
 * AS6500 Time-to-Digital Converter Arduino Driver Implementation
 */

#include "AS6500.h"

AS6500::AS6500(uint8_t csPin) {
    _csPin = csPin;
    _initialized = false;
    _calibrationValue = 0;
}

bool AS6500::begin(uint32_t spiSpeed) {
    // Initialize CS pin
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    
    // Initialize SPI
    SPI.begin();
    _spiSettings = SPISettings(spiSpeed, MSBFIRST, SPI_MODE0);
    
    // Small delay for power-up
    delay(10);
    
    // Reset the device
    reset();
    delay(10);
    
    // Check chip ID
    uint8_t chipID = getChipID();
    if (chipID != AS6500_CHIP_ID_VALUE) {
        return false;
    }
    
    // Perform initial calibration
    if (!calibrate()) {
        return false;
    }
    
    // Clear any pending interrupts
    clearInterrupts();
    
    _initialized = true;
    return true;
}

void AS6500::reset() {
    writeRegister(AS6500_REG_CONFIG0, AS6500_CONFIG0_RESET);
    delay(10);
    writeRegister(AS6500_REG_CONFIG0, 0x00);
}

void AS6500::enable(bool enable) {
    uint8_t config = readRegister(AS6500_REG_CONFIG0);
    if (enable) {
        config |= AS6500_CONFIG0_ENABLE;
    } else {
        config &= ~AS6500_CONFIG0_ENABLE;
    }
    writeRegister(AS6500_REG_CONFIG0, config);
}

bool AS6500::calibrate() {
    // Start calibration
    uint8_t config = readRegister(AS6500_REG_CONFIG0);
    config |= AS6500_CONFIG0_CALIBRATE;
    writeRegister(AS6500_REG_CONFIG0, config);
    
    // Wait for calibration to complete
    unsigned long startTime = millis();
    while (millis() - startTime < AS6500_CALIBRATION_TIME_MS) {
        uint8_t status = getInterruptStatus();
        if (status & AS6500_INT_CALIBRATION_DONE) {
            clearInterrupts(AS6500_INT_CALIBRATION_DONE);
            _calibrationValue = getCalibrationValue();
            return true;
        }
        delay(1);
    }
    
    return false; // Timeout
}

void AS6500::setMode(AS6500_Mode mode) {
    uint8_t config = readRegister(AS6500_REG_CONFIG0);
    config &= ~AS6500_CONFIG0_MODE_MASK;
    if (mode == MODE_CONTINUOUS) {
        config |= AS6500_CONFIG0_MODE_2;
    }
    writeRegister(AS6500_REG_CONFIG0, config);
}

void AS6500::setTimeout(uint8_t timeout) {
    if (timeout > 15) timeout = 15;
    uint8_t config = readRegister(AS6500_REG_CONFIG1);
    config &= ~AS6500_CONFIG1_TIMEOUT_MASK;
    config |= (timeout & AS6500_CONFIG1_TIMEOUT_MASK);
    writeRegister(AS6500_REG_CONFIG1, config);
}

void AS6500::setAveragingCycles(uint8_t cycles) {
    if (cycles > 3) cycles = 3;
    uint8_t config = readRegister(AS6500_REG_CONFIG1);
    config &= ~AS6500_CONFIG1_AVG_CYCLES;
    config |= ((cycles << 4) & AS6500_CONFIG1_AVG_CYCLES);
    writeRegister(AS6500_REG_CONFIG1, config);
}

void AS6500::startMeasurement() {
    writeRegister(AS6500_REG_FIRE_NUM, 0x01);
}

bool AS6500::isResultReady() {
    uint8_t status = getInterruptStatus();
    return (status & AS6500_INT_NEW_RESULT) != 0;
}

bool AS6500::readResult(AS6500_Result* result) {
    if (!isResultReady()) {
        result->valid = false;
        return false;
    }
    
    // Read 4 bytes of result data
    uint8_t data[4];
    readMultipleRegisters(AS6500_REG_TIME_RESULT_0, data, 4);
    
    // Combine bytes into 32-bit value
    result->timeValue = ((uint32_t)data[3] << 24) | 
                       ((uint32_t)data[2] << 16) | 
                       ((uint32_t)data[1] << 8) | 
                       data[0];
    
    result->valid = true;
    result->channelNum = 0;
    
    // Clear the result ready flag
    clearInterrupts(AS6500_INT_NEW_RESULT);
    
    return true;
}

uint32_t AS6500::getTimePicoseconds() {
    AS6500_Result result;
    if (readResult(&result)) {
        return result.timeValue;
    }
    return 0;
}

float AS6500::getTimeNanoseconds() {
    uint32_t picoseconds = getTimePicoseconds();
    return picoseconds / 1000.0;
}

uint8_t AS6500::getInterruptStatus() {
    return readRegister(AS6500_REG_INT_STATUS);
}

void AS6500::clearInterrupts(uint8_t flags) {
    writeRegister(AS6500_REG_INT_STATUS, flags);
}

void AS6500::setInterruptMask(uint8_t mask) {
    writeRegister(AS6500_REG_INT_MASK, mask);
}

uint8_t AS6500::getChipID() {
    return readRegister(AS6500_REG_CHIP_ID);
}

uint16_t AS6500::getCalibrationValue() {
    uint8_t data[2];
    readMultipleRegisters(AS6500_REG_CALIBRATION_RES, data, 2);
    return ((uint16_t)data[1] << 8) | data[0];
}

void AS6500::writeRegister(uint8_t reg, uint8_t value) {
    _beginTransaction();
    SPI.transfer(AS6500_CMD_WRITE | (reg & 0x7F));
    SPI.transfer(value);
    _endTransaction();
}

uint8_t AS6500::readRegister(uint8_t reg) {
    _beginTransaction();
    SPI.transfer(AS6500_CMD_READ | (reg & 0x7F));
    uint8_t value = SPI.transfer(0x00);
    _endTransaction();
    return value;
}

void AS6500::readMultipleRegisters(uint8_t reg, uint8_t* buffer, uint8_t length) {
    _beginTransaction();
    SPI.transfer(AS6500_CMD_READ | (reg & 0x7F));
    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = SPI.transfer(0x00);
    }
    _endTransaction();
}

void AS6500::_beginTransaction() {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);
}

void AS6500::_endTransaction() {
    delayMicroseconds(1);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}