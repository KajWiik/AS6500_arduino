/**
 * AS6500 Time-to-Digital Converter - Continuous Mode Example
 * 
 * This example demonstrates continuous time measurements
 */

#include <AS6500.h>

#define CS_PIN 10

AS6500 tdc(CS_PIN);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println("AS6500 Continuous Mode Test");
    Serial.println("============================");
    
    if (!tdc.begin()) {
        Serial.println("Initialization failed!");
        while (1);
    }
    
    // Configure for continuous mode
    tdc.setMode(MODE_CONTINUOUS);
    tdc.setTimeout(8);
    tdc.setAveragingCycles(1); // 2 averages
    tdc.enable(true);
    
    Serial.println("Starting continuous measurements...");
}

void loop() {
    if (tdc.isResultReady()) {
        float timeNs = tdc.getTimeNanoseconds();
        
        Serial.print("Time: ");
        Serial.print(timeNs, 3);
        Serial.println(" ns");
    }
    
    delay(10); // Small delay to prevent serial buffer overflow
}