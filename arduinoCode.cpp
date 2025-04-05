/* CSCI 3515 Lab 10 STA/LTA Code
    Final code for arduino */

// Include necessary libraries
#include <Wire.h>               // For I2C
#include <LiquidCrystal.h>      // For LCD (assuming using standard 6-pin parallel)

// MPU6050 I2C address
const int MPU_ADDR = 0x68;

// LCD pins: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// STA/LTA parameters
const int STA_WINDOW = 5;
const int LTA_WINDOW = 20;
const float TRIGGER_THRESHOLD = 3.5;
const float DETRIGGER_THRESHOLD = 1.0;

// Sliding window buffers
float staSum = 0;
float ltaSum = 0;
float staValues[STA_WINDOW] = {0};
float ltaValues[LTA_WINDOW] = {0};

int staIndex = 0;
int ltaIndex = 0;

bool detected = false;
int stepCount = 0;

// Temporary variable to hold sensor reading
float currentAccY = 0.0;

void setup() {
    Wire.begin();
    Serial.begin(9600);
  
    // Wake up MPU6050
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // Set to zero (wakes up MPU6050)
    Wire.endTransmission(true);

    // Initialize LCD
    lcd.begin(16, 2);
    lcd.print("Steps: 0");
}

void loop() {
    // Read acceleration Y axis from MPU6050
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3D);  // ACCEL_YOUT_H register
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 2, true);
    int16_t rawAccY = Wire.read() << 8 | Wire.read();
    currentAccY = rawAccY / 16384.0;  // Convert to "g" units

    // Preprocess: use absolute value like your C++ code
    float x = abs(currentAccY);

    // Update STA and LTA sliding windows
    staSum = staSum - staValues[staIndex] + x;
    staValues[staIndex] = x;
    staIndex = (staIndex + 1) % STA_WINDOW;
    
    ltaSum = ltaSum - ltaValues[ltaIndex] + x;
    ltaValues[ltaIndex] = x;
    ltaIndex = (ltaIndex + 1) % LTA_WINDOW;

    // Only calculate STA/LTA ratio when windows are filled
    if (ltaSum > 0) {
        float sta = staSum / STA_WINDOW;
        float lta = ltaSum / LTA_WINDOW;
        float ratio = sta / lta;

        // Step detection logic
        if (!detected && ratio > TRIGGER_THRESHOLD) {
        detected = true;
        stepCount++;
        
        // Update LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Steps:");
        lcd.setCursor(0, 1);
        lcd.print(stepCount);
        } else if (detected && ratio < DETRIGGER_THRESHOLD) {
        detected = false;
        }
    }

    delay(50);  // Adjust as needed (50ms = ~20Hz)
}
