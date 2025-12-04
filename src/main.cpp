#include <Arduino.h>
#include "Config.h"
#include "UltrasonicSensor.h"
#include "DisplayManager.h"
#include "PrayerStateMachine.h"
#include "RakaatCounter.h"

// ===== MODULE INSTANCES =====
UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN);
DisplayManager display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RESET, OLED_ADDRESS);
PrayerStateMachine stateMachine(STABLE_TIME, RELEASE_TIME);
RakaatCounter counter;

// ===== TIMING VARIABLES =====
unsigned long lastSensorRead = 0;

void setup() {
  // Initialize Serial for debugging
  #if DEBUG_MODE
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("============================"));
  Serial.println(F("Smart Rakaat Tracker v1.0"));
  Serial.println(F("============================"));
  #endif
  
  // Initialize Ultrasonic Sensor
  sensor.begin();
  #if DEBUG_MODE
  Serial.println(F("Ultrasonic Sensor initialized"));
  #endif
  
  // Initialize OLED Display
  if (!display.begin()) {
    #if DEBUG_MODE
    Serial.println(F("Display initialization failed!"));
    #endif
    while (1); // Halt if display fails
  }
  #if DEBUG_MODE
  Serial.println(F("Display initialized"));
  #endif
  
  // Show Boot Screen
  display.showBootScreen();
  delay(BOOT_SCREEN_DURATION);
  
  // Initialize State Machine and Counter
  stateMachine.begin();
  counter.begin();
  
  // Display initial state
  display.updateRakaatDisplay(counter.getCurrentRakaat(), counter.getSujoodCount());
  
  #if DEBUG_MODE
  Serial.println(F("System Ready!"));
  Serial.println(F("Place device at the head of prayer mat"));
  Serial.println(F("Sensor pointing upward toward forehead position"));
  Serial.println(F("============================\n"));
  #endif
}

void loop() {
  unsigned long currentTime = millis();
  
  // Read sensor at specified interval (10Hz by default)
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    lastSensorRead = currentTime;
    
    // Check if distance is below threshold (object detected)
    bool isObjectDetected = sensor.isDistanceBelow(SUJOOD_THRESHOLD);
    
    #if DEBUG_MODE
    float distance = sensor.measureDistance();
    static unsigned long lastDebugPrint = 0;
    if (currentTime - lastDebugPrint >= 1000) { // Print every second
      Serial.print(F("Distance: "));
      Serial.print(distance);
      Serial.print(F(" cm | State: "));
      Serial.println(stateMachine.getStateName());
      lastDebugPrint = currentTime;
    }
    #endif
    
    // Process detection through state machine
    bool sujoodValidated = stateMachine.processDetection(isObjectDetected);
    
    // If a Sujood was validated, increment counter and update display
    if (sujoodValidated) {
      counter.incrementSujood();
      display.updateRakaatDisplay(counter.getCurrentRakaat(), counter.getSujoodCount());
      
      #if DEBUG_MODE
      Serial.println(F("\n*** SUJOOD VALIDATED ***"));
      Serial.print(F("Total Sujoods: "));
      Serial.println(counter.getSujoodCount());
      Serial.print(F("Current Rakaat: "));
      Serial.println(counter.getCurrentRakaat());
      Serial.println(F("************************\n"));
      #endif
    }
  }
}
