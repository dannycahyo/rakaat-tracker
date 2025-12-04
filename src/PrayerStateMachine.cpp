#include "PrayerStateMachine.h"
#include "Config.h"

PrayerStateMachine::PrayerStateMachine(unsigned long stableMs, unsigned long releaseMs) 
    : stableTime(stableMs), releaseTime(releaseMs) {
    currentState = STATE_IDLE;
    triggerStartTime = 0;
    releaseStartTime = 0;
    sujoodJustValidated = false;
}

void PrayerStateMachine::begin() {
    currentState = STATE_IDLE;
    triggerStartTime = 0;
    releaseStartTime = 0;
    sujoodJustValidated = false;
}

bool PrayerStateMachine::processDetection(bool isObjectDetected) {
    unsigned long currentTime = millis();
    bool sujoodValidated = false;
    
    switch (currentState) {
        case STATE_IDLE:
            if (isObjectDetected) {
                // Object detected, start timer
                currentState = STATE_TRIGGERED;
                triggerStartTime = currentTime;
                
                #if DEBUG_MODE
                Serial.println(F("State: IDLE -> TRIGGERED"));
                #endif
            }
            break;
            
        case STATE_TRIGGERED:
            if (!isObjectDetected) {
                // Object moved away before validation
                currentState = STATE_IDLE;
                triggerStartTime = 0;
                
                #if DEBUG_MODE
                Serial.println(F("State: TRIGGERED -> IDLE (signal lost)"));
                #endif
            } 
            else if (currentTime - triggerStartTime >= stableTime) {
                // Object stayed long enough - Validate Sujood
                currentState = STATE_VALIDATED;
                sujoodValidated = true;
                
                #if DEBUG_MODE
                Serial.println(F("State: TRIGGERED -> VALIDATED (Sujood confirmed!)"));
                #endif
            }
            break;
            
        case STATE_VALIDATED:
            if (!isObjectDetected) {
                // User is rising from Sujood
                currentState = STATE_RELEASE;
                releaseStartTime = currentTime;
                
                #if DEBUG_MODE
                Serial.println(F("State: VALIDATED -> RELEASE"));
                #endif
            }
            // Stay in VALIDATED while object is still detected to prevent double counting
            break;
            
        case STATE_RELEASE:
            if (currentTime - releaseStartTime >= releaseTime) {
                // Release period complete, return to idle
                currentState = STATE_IDLE;
                
                #if DEBUG_MODE
                Serial.println(F("State: RELEASE -> IDLE (Ready for next Sujood)"));
                #endif
            }
            // If object detected again during release, go back to triggered
            else if (isObjectDetected) {
                currentState = STATE_TRIGGERED;
                triggerStartTime = currentTime;
                
                #if DEBUG_MODE
                Serial.println(F("State: RELEASE -> TRIGGERED (Re-detected)"));
                #endif
            }
            break;
    }
    
    return sujoodValidated;
}

PrayerState PrayerStateMachine::getCurrentState() {
    return currentState;
}

void PrayerStateMachine::reset() {
    begin();
}

#if DEBUG_MODE
const char* PrayerStateMachine::getStateName() {
    switch (currentState) {
        case STATE_IDLE: return "IDLE";
        case STATE_TRIGGERED: return "TRIGGERED";
        case STATE_VALIDATED: return "VALIDATED";
        case STATE_RELEASE: return "RELEASE";
        default: return "UNKNOWN";
    }
}
#endif
