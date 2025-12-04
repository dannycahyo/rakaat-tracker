#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trig, uint8_t echo) 
    : trigPin(trig), echoPin(echo) {}

void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

float UltrasonicSensor::measureDistance() {
    // Clear the trigger pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Send a 10 microsecond pulse
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read echo pin with timeout (38ms for ~6.5m range)
    long duration = pulseIn(echoPin, HIGH, 38000);
    
    // If timeout occurs, return max distance
    if (duration == 0) {
        return 999.0;
    }
    
    // Calculate distance in cm
    float distance = duration * 0.0343 / 2;
    
    return distance;
}

bool UltrasonicSensor::isDistanceBelow(float threshold) {
    float distance = measureDistance();
    return distance < threshold;
}
