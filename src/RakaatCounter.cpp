#include "RakaatCounter.h"
#include "Config.h"

RakaatCounter::RakaatCounter() {
    sujoodCount = 0;
    currentRakaat = 0;
}

void RakaatCounter::begin() {
    sujoodCount = 0;
    currentRakaat = 0;
}

void RakaatCounter::incrementSujood() {
    sujoodCount++;
    calculateRakaat();
    
    #if DEBUG_MODE
    Serial.print(F("Sujood Count: "));
    Serial.print(sujoodCount);
    Serial.print(F(" | Current Rakaat: "));
    Serial.println(currentRakaat);
    #endif
}

int RakaatCounter::getSujoodCount() {
    return sujoodCount;
}

int RakaatCounter::getCurrentRakaat() {
    return currentRakaat;
}

void RakaatCounter::reset() {
    begin();
}

void RakaatCounter::calculateRakaat() {
    // Logic: Current Rakaat = Total Valid Sujoods / 2
    // This means:
    // - 0 Sujoods = Rakaat 0 (starting/not yet begun)
    // - 1 Sujood = Rakaat 0 (first Sujood completed)
    // - 2 Sujoods = Rakaat 1 (first Rakaat completed)
    // - 3 Sujoods = Rakaat 1 (first Sujood of second Rakaat)
    // - 4 Sujoods = Rakaat 2 (second Rakaat completed)
    currentRakaat = sujoodCount / 2;
}
