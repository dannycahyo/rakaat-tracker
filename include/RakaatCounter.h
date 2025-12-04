#ifndef RAKAAT_COUNTER_H
#define RAKAAT_COUNTER_H

#include <Arduino.h>

class RakaatCounter {
private:
    int sujoodCount;
    int currentRakaat;
    
public:
    RakaatCounter();
    
    void begin();
    void incrementSujood();
    int getSujoodCount();
    int getCurrentRakaat();
    void reset();
    
private:
    void calculateRakaat();
};

#endif
