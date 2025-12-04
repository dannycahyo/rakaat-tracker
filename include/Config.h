#ifndef CONFIG_H
#define CONFIG_H

// ===== HARDWARE PIN CONFIGURATION =====
#define TRIG_PIN 5
#define ECHO_PIN 18
#define OLED_SDA 21
#define OLED_SCL 22

// ===== OLED DISPLAY CONFIGURATION =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// ===== DETECTION THRESHOLDS =====
#define SUJOOD_THRESHOLD 30.0      // Distance in cm to detect Sujood
#define STABLE_TIME 1500           // Time in ms to validate Sujood (1.5 seconds)
#define RELEASE_TIME 500           // Time in ms before returning to idle after rising
#define SENSOR_READ_INTERVAL 100   // Read sensor every 100ms (10Hz)

// ===== SERIAL CONFIGURATION =====
#define SERIAL_BAUD 115200
#define DEBUG_MODE true            // Set to false to disable debug prints

// ===== DISPLAY TIMING =====
#define BOOT_SCREEN_DURATION 2000  // Boot screen display time in ms

#endif
