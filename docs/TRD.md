## **Technical Requirement Document (TRD)**

**Project Name:** Smart Rakaat Tracker (MVP)
**Device Type:** Standalone IoT Edge Device
**Version:** 1.0 (MVP)

### **1. Executive Summary**

The Smart Rakaat Tracker is a non-intrusive, digital counter designed to assist Muslims during Salah (prayer) by automatically tracking the number of Rakaats performed.

The MVP (Minimum Viable Product) aims to solve the problem of _Sahw_ (forgetfulness) by using ultrasonic distance sensing to detect the specific physical movement of _Sujood_ (prostration). The device will process this data locally on an ESP32 and display the current Rakaat count on an OLED screen.

### **2. System Architecture Overview**

The system operates as a **Standalone Edge Device**. It does not require internet connectivity, a backend server, or a mobile app for this MVP phase.

**Data Flow Pipeline:**

1.  **Input:** The Ultrasonic Sensor emits sound waves to measure the distance to the nearest object (the user's head during Sujood).
2.  **Processing:** The ESP32 Microcontroller continuously reads this distance, applies a "debounce" algorithm to filter noise, and determines if a valid Sujood has occurred.
3.  **Logic:** A Finite State Machine (FSM) tracks the sequence of movements to increment the Rakaat counter.
4.  **Output:** The OLED Display updates in real-time to show the current Rakaat number.

### **3. Hardware Requirements**

Based on the provided **IoT Starter Toolkit**, the following hardware configuration is required.

#### **3.1 Bill of Materials (BOM)**

| Component                       | Function        | Justification                                                                                                              |
| :------------------------------ | :-------------- | :------------------------------------------------------------------------------------------------------------------------- |
| **ESP32 Dev Module**            | Main Controller | Handles logic, I/O processing, and future-proofing for WiFi features.                                                      |
| **Ultrasonic Sensor (HC-SR04)** | Proximity Input | Detects the presence of the user's head within a specific range (<30cm).                                                   |
| **OLED Display (0.96" I2C)**    | Visual Output   | High contrast, self-emitting light (visible in dark), low power.                                                           |
| **Breadboard & Jumpers**        | Prototyping     | Connects components without soldering.                                                                                     |
| **Power Supply (MB102)**        | Power Source    | Ensures stable 5V to the Ultrasonic sensor (ESP32 USB power can sometimes be insufficient for stable ultrasonic readings). |

#### **3.2 Hardware Interfaces & Wiring Strategy**

- **I2C Bus:** Used for the OLED Display (SDA/SCL pins).
- **GPIO Digital I/O:**
  - **Trigger Pin (Output):** Sends the ultrasonic pulse.
  - **Echo Pin (Input):** Receives the reflected pulse.

### **4. Functional Requirements**

#### **4.1 Initialization**

- **FR-01:** Upon power-up, the system must initialize the Serial communication (for debug), the I2C display, and the sensor pins.
- **FR-02:** The display must show a "Boot/Ready" screen for 2 seconds before switching to the main counter interface.
- **FR-03:** The default starting variables must be `Sujood Count = 0` and `Current Rakaat = 1`.

#### **4.2 Detection Logic (The "Eye")**

- **FR-04:** The sensor must ping for distance data at a frequency of at least 10Hz (every 100ms) to ensure responsiveness.
- **FR-05:** The system must define a **`SUJOOD_THRESHOLD`** (e.g., 30cm). Any reading below this distance is flagged as a "Potential Sujood."

#### **4.3 Validation Logic (The "Brain")**

- **FR-06 (Debouncing):** To prevent false counting (e.g., a hand waving in front of the sensor), the system must implement a time-based filter.
  - _Requirement:_ The distance must remain _below_ the threshold continuously for a minimum of **1.5 seconds** (configurable `STABLE_TIME`).
- **FR-07:** If the object moves away (distance > threshold) before the 1.5-second timer completes, the timer resets, and no count is registered.

#### **4.4 Counting Logic**

- **FR-08:** The system interprets Rakaats based on Sujood pairs.
  - Logic: `Current Rakaat = (Total Valid Sujoods / 2) + 1` (Integer math).
  - _Note:_ The user starts at Rakaat 1. After 2 Sujoods are completed, the counter transitions to indicate the user is now approaching or in Rakaat 2.

#### **4.5 User Interface (OLED)**

- **FR-09:** The Main Screen must be simple and readable from a standing distance (approx. 1.5 - 1.7 meters).
- **FR-10:** **Primary Element:** The "Current Rakaat" number must be the largest font size available.
- **FR-11:** **Secondary Element:** Small debug text at the bottom showing "Sujood Count" (helps the user trust the device is working).

### **5. Firmware Logic Flow (State Machine)**

To ensure stability, the firmware will implement a Finite State Machine with the following states:

1.  **STATE_IDLE (Standing/Sitting):**

    - Sensor reads distance > `SUJOOD_THRESHOLD`.
    - System waits.

2.  **STATE_TRIGGERED (Going Down):**

    - Sensor reads distance < `SUJOOD_THRESHOLD`.
    - Internal timer starts.
    - If signal is lost, return to STATE_IDLE immediately.

3.  **STATE_VALIDATED (In Sujood):**

    - Internal timer exceeds `STABLE_TIME` (1.5s).
    - **Action:** Increment `Sujood_Counter`.
    - **Action:** Recalculate `Rakaat_Counter`.
    - **Action:** Update Display.
    - System locks in this state to prevent double-counting until the user rises.

4.  **STATE_RELEASE (Rising):**
    - Sensor reads distance > `SUJOOD_THRESHOLD`.
    - System pauses briefly (e.g., 0.5s) to allow the user to fully sit up before returning to STATE_IDLE.

### **6. Physical Implementation & Calibration**

#### **6.1 Sensor Placement**

- The device is designed to be placed on the floor at the **head of the prayer mat**.
- The Ultrasonic Sensor must point **upwards** (Vertical) or angled slightly toward the user's forehead position.

#### **6.2 Calibration Requirement**

- Before relying on the device, a calibration test is required to define the `SUJOOD_THRESHOLD`.
- _Method:_ The user must perform a test Sujood to measure the specific distance between the floor-mounted sensor and their forehead. The firmware threshold should be set roughly 5-10cm _higher_ than this measured distance to ensure reliable detection.

### **7. Limitations & Constraints (MVP)**

- **No Prayer Selection:** The device assumes a generic counter starting from 1. It does not know if the user is praying Fajr (2 Rakaat) or Isha (4 Rakaat).
- **No Auto-Reset:** To reset the counter for a new prayer, the user must power cycle the device or press the "EN" (Reset) button on the ESP32.
- **Interference:** Loose clothing (Hijab/Keffiyeh) falling over the sensor _before_ the head reaches the ground may trigger the sensor early. The 1.5s delay (FR-06) mitigates this but does not eliminate it.
