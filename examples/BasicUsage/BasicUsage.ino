/**
 * AS6500 Time-to-Digital Converter - Basic Usage Example
 * 
 * This example demonstrates basic time measurement using the AS6500 TDC
 * 
 * Hardware Connections (Arduino):
 * - CS   -> Pin 10 (or any digital pin)
 * - MOSI -> Pin 11
 * - MISO -> Pin 12
 * - SCK  -> Pin 13
 * - VDD  -> 3.3V
 * - GND  -> GND
 */

#include <AS6500.h>

// Define CS pin
#define CS_PIN 10

// Create AS6500 object
AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    
    Serial.println("AS6500 Time-to-Digital Converter Test");
    Serial.println("======================================");
    
    // Initialize the TDC
    if (tdc.begin()) {
        Serial.println("AS6500 initialized successfully!");
    } else {
        Serial.println("Failed to initialize AS6500!");
        Serial.println("Check your wiring and try again.");
        while (1) {
            delay(1000);
        }
    }
    
    // Read and display chip ID
    uint8_t chipID = tdc.getChipID();
    Serial.print("Chip ID: 0x");
    Serial.println(chipID, HEX);
    
    // Configure the TDC
    tdc.setMode(MODE_SINGLE_SHOT);
    tdc.setTimeout(10);
    tdc.setAveragingCycles(2); // 4 averages
    
    // Enable the TDC
    tdc.enable(true);
    
    Serial.println("Configuration complete. Starting measurements...");
    Serial.println();
}

void loop() {
    // Start a measurement
    tdc.startMeasurement();
    
    // Wait for result (with timeout)
    unsigned long startTime = millis();
    while (!tdc.isResultReady()) {
        if (millis() - startTime > 1000) {
            Serial.println("Measurement timeout!");
            delay(1000);
            return;
        }
        delay(1);
    }
    
    // Read the result
    AS6500_Result result;
    if (tdc.readResult(&result)) {
        Serial.print("Time measurement: ");
        Serial.print(result.timeValue);
        Serial.print(" ps (");
        Serial.print(result.timeValue / 1000.0, 3);
        Serial.println(" ns)");
    } else {
        Serial.println("Failed to read result!");
    }
    
    // Check for errors
    uint8_t status = tdc.getInterruptStatus();
    if (status & AS6500_INT_ERROR) {
        Serial.println("Error detected!");
        tdc.clearInterrupts(AS6500_INT_ERROR);
    }
    
    if (status & AS6500_INT_TIMEOUT) {
        Serial.println("Measurement timeout!");
        tdc.clearInterrupts(AS6500_INT_TIMEOUT);
    }
    
    delay(500); // Delay between measurements
}