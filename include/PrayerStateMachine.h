#ifndef PRAYER_STATE_MACHINE_H
#define PRAYER_STATE_MACHINE_H

#include <Arduino.h>
#include "Config.h"

enum PrayerState {
    STATE_IDLE,       // Standing/Sitting - No detection
    STATE_TRIGGERED,  // Object detected, timer started
    STATE_VALIDATED,  // Sujood confirmed, count incremented
    STATE_RELEASE     // Rising from Sujood, waiting to return to idle
};

class PrayerStateMachine {
private:
    PrayerState currentState;
    unsigned long triggerStartTime;
    unsigned long releaseStartTime;
    unsigned long stableTime;
    unsigned long releaseTime;
    
    bool sujoodJustValidated;  // Flag to prevent double counting
    
public:
    PrayerStateMachine(unsigned long stableMs, unsigned long releaseMs);
    
    void begin();
    bool processDetection(bool isObjectDetected);
    PrayerState getCurrentState();
    void reset();
    
    #if DEBUG_MODE
    const char* getStateName();
    #endif
};

#endif
