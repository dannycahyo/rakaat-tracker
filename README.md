# Smart Rakaat Tracker

A non-intrusive IoT device that automatically tracks the number of Rakaats performed during Islamic prayer (Salah) using ultrasonic distance sensing.

![Version](https://img.shields.io/badge/version-1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

---

## 📖 Overview

The Smart Rakaat Tracker solves the problem of _Sahw_ (forgetfulness during prayer) by using an **HC-SR04 ultrasonic sensor** to detect _Sujood_ (prostration) movements. The device processes data locally on an **ESP32 microcontroller** and displays the current Rakaat count on an **OLED screen** in real-time.

### Key Features

✅ **Automatic Detection** - Detects Sujood using ultrasonic distance sensing  
✅ **Robust Validation** - 1.5-second debounce filter to prevent false counts  
✅ **Real-time Display** - OLED screen shows current Rakaat and Sujood count  
✅ **Standalone Device** - No WiFi, server, or mobile app required  
✅ **Clean Architecture** - Modular code with separation of concerns  
✅ **Debug Mode** - Detailed serial logging for calibration and troubleshooting

---

## 🛠️ Hardware Requirements

### Bill of Materials (BOM)

| Component             | Specification      | Purpose                                   |
| --------------------- | ------------------ | ----------------------------------------- |
| **ESP32 Dev Module**  | ESP32-WROOM-32     | Main microcontroller                      |
| **Ultrasonic Sensor** | HC-SR04            | Distance measurement for Sujood detection |
| **OLED Display**      | 0.96" I2C (128x64) | Visual output for Rakaat count            |
| **Breadboard**        | Standard           | Prototyping connections                   |
| **Jumper Wires**      | Male-to-Male       | Wiring components                         |
| **Power Supply**      | 5V USB or MB102    | Power for ESP32 and sensor                |

### Wiring Diagram

```
ESP32          HC-SR04          OLED (I2C)
-----          -------          ----------
GPIO 5    -->  TRIG
GPIO 18   <--  ECHO
GPIO 21   <--------------->    SDA
GPIO 22   <--------------->    SCL
GND       ---  GND       ---    GND
5V        ---  VCC       ---    VCC
```

**Pin Configuration:**

- **Ultrasonic Sensor:** TRIG → GPIO 5, ECHO → GPIO 18
- **OLED Display:** SDA → GPIO 21, SCL → GPIO 22

---

## 🚀 Getting Started

### Prerequisites

- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) (VS Code extension)
- USB cable for ESP32
- Basic understanding of electronics wiring

### Installation

1. **Clone or download this project**

   ```bash
   cd /path/to/rakaat-tracker
   ```

2. **Open in PlatformIO**

   - Open VS Code
   - File → Open Folder → Select `rakaat-tracker`

3. **Install dependencies** (automatic)

   - PlatformIO will auto-install:
     - `Adafruit SSD1306` library
     - `Adafruit GFX Library`

4. **Wire the hardware** according to the diagram above

5. **Build and upload**

   ```bash
   pio run --target upload
   ```

6. **Open Serial Monitor** (optional, for debugging)
   ```bash
   pio device monitor
   ```

---

## ⚙️ Configuration

All settings are centralized in `include/Config.h`:

```cpp
// Detection Thresholds
#define SUJOOD_THRESHOLD 30.0      // Distance in cm (adjust based on your height)
#define STABLE_TIME 1500           // Validation time in ms (1.5 seconds)
#define RELEASE_TIME 500           // Cooldown after rising (0.5 seconds)
#define SENSOR_READ_INTERVAL 100   // Sensor polling rate (10Hz)

// Debug Mode
#define DEBUG_MODE true            // Set to false to disable serial logging
```

### Calibration

1. Place the device on the floor at the **head of your prayer mat**
2. Point the ultrasonic sensor **upward** (vertical or slightly angled)
3. Enable `DEBUG_MODE true` in `Config.h`
4. Upload and monitor serial output
5. Perform a test Sujood and note the distance reading
6. Set `SUJOOD_THRESHOLD` to ~5-10cm above your measured distance
7. Test again to ensure reliable detection

---

## 🧠 How It Works

### System Architecture

The device uses a **Finite State Machine (FSM)** to ensure robust Sujood detection:

```
┌─────────────┐
│   IDLE      │ ◄─── Distance > 30cm (Standing/Sitting)
└──────┬──────┘
       │ Distance < 30cm detected
       ▼
┌─────────────┐
│  TRIGGERED  │ ◄─── Timer started, monitoring stability
└──────┬──────┘
       │ Stable for 1.5s
       ▼
┌─────────────┐
│  VALIDATED  │ ◄─── Sujood confirmed! Counter incremented
└──────┬──────┘
       │ Distance > 30cm (User rising)
       ▼
┌─────────────┐
│   RELEASE   │ ◄─── Cooldown period (0.5s)
└──────┬──────┘
       │
       └──────► Back to IDLE
```

### Counting Logic

- **Rakaat Calculation:** `Current Rakaat = Total Sujoods / 2`
  - 0 Sujoods → Rakaat 0 (starting position)
  - 2 Sujoods → Rakaat 1 (first Rakaat completed)
  - 4 Sujoods → Rakaat 2 (second Rakaat completed)
  - 6 Sujoods → Rakaat 3, etc.

### Module Overview

| Module                 | Responsibility                              |
| ---------------------- | ------------------------------------------- |
| **Config.h**           | Hardware pins, thresholds, timing constants |
| **UltrasonicSensor**   | HC-SR04 hardware abstraction                |
| **DisplayManager**     | OLED display control (Adafruit libraries)   |
| **PrayerStateMachine** | FSM logic for Sujood detection              |
| **RakaatCounter**      | Sujood counting and Rakaat calculation      |
| **main.cpp**           | System orchestration and main loop          |

---

## 📂 Project Structure

```
rakaat-tracker/
├── include/
│   ├── Config.h                 # Central configuration
│   ├── UltrasonicSensor.h       # Sensor interface
│   ├── DisplayManager.h         # OLED display management
│   ├── PrayerStateMachine.h     # FSM for Sujood detection
│   └── RakaatCounter.h          # Rakaat counting logic
├── src/
│   ├── main.cpp                 # Main application
│   ├── UltrasonicSensor.cpp     # Sensor implementation
│   ├── DisplayManager.cpp       # Display implementation
│   ├── PrayerStateMachine.cpp   # State machine implementation
│   └── RakaatCounter.cpp        # Counter implementation
├── docs/
│   └── TRD.md                   # Technical Requirements Document
├── platformio.ini               # PlatformIO configuration
└── README.md                    # This file
```

---

## 🎯 Usage Instructions

1. **Power on** the device
2. **Wait for boot screen** (2 seconds)
3. **Place at prayer mat head** with sensor pointing upward
4. **Begin prayer** - the device will automatically detect and count
5. **Monitor OLED** - displays current Rakaat in large font
6. **Reset for new prayer** - press the **EN button** on ESP32

### Display Interface

```
┌─────────────────┐
│                 │
│       2         │  ← Current Rakaat (large font)
│                 │
│     RAKAAT      │  ← Label
│                 │
│  Sujood: 4      │  ← Debug counter
└─────────────────┘
```

---

## 🐛 Troubleshooting

| Issue                  | Solution                                                 |
| ---------------------- | -------------------------------------------------------- |
| **No display output**  | Check I2C wiring (SDA/SCL), verify OLED address is 0x3C  |
| **False detections**   | Increase `STABLE_TIME` or adjust `SUJOOD_THRESHOLD`      |
| **Missed detections**  | Decrease `SUJOOD_THRESHOLD`, ensure sensor points upward |
| **Erratic readings**   | Check sensor wiring, ensure stable 5V power supply       |
| **Compilation errors** | Run `pio lib install` to ensure libraries are installed  |

### Debug Mode

Enable detailed logging by setting `DEBUG_MODE true` in `Config.h`:

```
============================
Smart Rakaat Tracker v1.0
============================
Ultrasonic Sensor initialized
Display initialized
System Ready!
============================

Distance: 45.2 cm | State: IDLE
Distance: 28.3 cm | State: TRIGGERED
State: TRIGGERED -> VALIDATED (Sujood confirmed!)

*** SUJOOD VALIDATED ***
Total Sujoods: 1
Current Rakaat: 0
************************
```

---

## 🔮 Future Enhancements

This modular architecture supports easy extensions:

- [ ] **WiFi Connectivity** - Cloud logging and prayer analytics
- [ ] **Button Input** - Manual reset without power cycle
- [ ] **Audio Feedback** - Buzzer confirmation on detection
- [ ] **Prayer Type Selection** - Auto-reset after 2/3/4 Rakaats
- [ ] **Battery Power** - Rechargeable Li-ion with power management
- [ ] **3D Printed Enclosure** - Professional housing design
- [ ] **Multiple Sensors** - Support for Jama'ah (group prayer)

---

## 📜 Technical Specifications

| Parameter              | Value                    |
| ---------------------- | ------------------------ |
| **Microcontroller**    | ESP32 (240MHz dual-core) |
| **Detection Range**    | 2-400 cm (HC-SR04)       |
| **Effective Range**    | 15-30 cm (configurable)  |
| **Scan Frequency**     | 10 Hz (100ms interval)   |
| **Validation Time**    | 1.5 seconds              |
| **Display Resolution** | 128x64 pixels (OLED)     |
| **Power Consumption**  | ~150mA @ 5V              |
| **Operating Voltage**  | 5V USB                   |

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

### Development Guidelines

1. Follow the existing code structure and naming conventions
2. Test thoroughly before submitting PRs
3. Update documentation for new features
4. Maintain separation of concerns across modules

---

## 📄 License

This project is open-source and available under the MIT License.

---

## 🙏 Acknowledgments

- **Inspiration:** Solving _Sahw_ (forgetfulness in prayer) for the Muslim community
- **Libraries:** Adafruit (SSD1306, GFX)
- **Platform:** PlatformIO & ESP32 ecosystem

---

## 📧 Contact

For questions, suggestions, or feedback, please open an issue on the project repository.

---

**May this device help maintain focus and tranquility in your prayers. Ameen.** 🤲
