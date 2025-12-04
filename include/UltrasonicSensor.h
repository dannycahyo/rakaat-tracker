#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
private:
    uint8_t trigPin;
    uint8_t echoPin;
    
public:
    UltrasonicSensor(uint8_t trig, uint8_t echo);
    void begin();
    float measureDistance();
    bool isDistanceBelow(float threshold);
};

#endif
