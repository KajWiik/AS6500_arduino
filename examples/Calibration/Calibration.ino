/**
 * AS6500 Time-to-Digital Converter - Calibration Example
 * 
 * This example demonstrates calibration procedures
 */

#include <AS6500.h>

#define CS_PIN 10

AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println("AS6500 Calibration Test");
    Serial.println("=======================");
    
    if (!tdc.begin()) {
        Serial.println("Initialization failed!");
        while (1);
    }
    
    Serial.println("Performing calibration...");
    
    if (tdc.calibrate()) {
        Serial.println("Calibration successful!");
        uint16_t calValue = tdc.getCalibrationValue();
        Serial.print("Calibration value: ");
        Serial.println(calValue);
    } else {
        Serial.println("Calibration failed!");
    }
    
    Serial.println("\nYou can perform recalibration by sending 'c' via serial.");
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == 'c' || cmd == 'C') {
            Serial.println("Recalibrating...");
            if (tdc.calibrate()) {
                Serial.println("Calibration successful!");
                uint16_t calValue = tdc.getCalibrationValue();
                Serial.print("Calibration value: ");
                Serial.println(calValue);
            } else {
                Serial.println("Calibration failed!");
            }
        }
    }
    
    delay(100);
}