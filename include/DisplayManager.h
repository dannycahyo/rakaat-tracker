#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayManager {
private:
    Adafruit_SSD1306* display;
    
public:
    DisplayManager(uint8_t width, uint8_t height, uint8_t reset, uint8_t address);
    ~DisplayManager();
    
    bool begin();
    void showBootScreen();
    void updateRakaatDisplay(int currentRakaat, int sujoodCount);
    void clear();
};

#endif
