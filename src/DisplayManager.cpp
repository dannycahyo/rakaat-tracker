#include "DisplayManager.h"
#include "Config.h"

DisplayManager::DisplayManager(uint8_t width, uint8_t height, uint8_t reset, uint8_t address) {
    display = new Adafruit_SSD1306(width, height, &Wire, reset);
}

DisplayManager::~DisplayManager() {
    delete display;
}

bool DisplayManager::begin() {
    if (!display->begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        #if DEBUG_MODE
        Serial.println(F("SSD1306 allocation failed"));
        #endif
        return false;
    }
    return true;
}

void DisplayManager::showBootScreen() {
    display->clearDisplay();
    display->setTextColor(SSD1306_WHITE);
    
    // Title
    display->setTextSize(2);
    display->setCursor(10, 10);
    display->println(F("RAKAAT"));
    display->setCursor(10, 30);
    display->println(F("TRACKER"));
    
    // Version
    display->setTextSize(1);
    display->setCursor(35, 50);
    display->println(F("v1.0 MVP"));
    
    display->display();
}

void DisplayManager::updateRakaatDisplay(int currentRakaat, int sujoodCount) {
    display->clearDisplay();
    
    // Main Rakaat Number (Large, Centered)
    display->setTextSize(4);
    display->setTextColor(SSD1306_WHITE);
    
    // Center the number
    int16_t x = (SCREEN_WIDTH - 24) / 2;  // Approximate width of large digit
    int16_t y = 15;
    
    display->setCursor(x, y);
    display->println(currentRakaat);
    
    // Label "Rakaat" below the number
    display->setTextSize(1);
    display->setCursor(40, 50);
    display->println(F("RAKAAT"));
    
    // Debug info at bottom (Sujood count)
    display->setTextSize(1);
    display->setCursor(0, 56);
    display->print(F("Sujood: "));
    display->print(sujoodCount);
    
    display->display();
}

void DisplayManager::clear() {
    display->clearDisplay();
    display->display();
}
