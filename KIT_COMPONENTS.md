# Arduino Starter Kit - Component Inventory & Capabilities

## The Mental Model for Software Engineers

Think of each component as a **module** or **library** with specific inputs/outputs and protocols.

---

## 1. CORE COMPONENTS

### Arduino UNO R4 Minima
**What it is:** The CPU/motherboard - runs your code
**Specs:**
- Renesas RA4M1 ARM Cortex-M4 @ 48 MHz
- 32 KB SRAM, 256 KB Flash
- 5V operating voltage
- 14 digital I/O pins (6 PWM)
- 6 analog input pins (10-bit ADC, 0-1023 range)
- USB-C programming interface

**Mental Model:** This is your server/runtime environment

---

## 2. INPUT COMPONENTS (Sensors - Read Data)

### Buttons / Push Switches
- **Type:** Digital Input
- **Returns:** HIGH (not pressed) or LOW (pressed)
- **Use Case:** User input, trigger events
- **Code Pattern:**
  ```cpp
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Button pressed
  }
  ```

### Potentiometer (Knob/Dial)
- **Type:** Analog Input
- **Returns:** 0-1023 (maps to rotation angle)
- **Use Case:** Volume control, brightness, user input
- **Mental Model:** Like a slider input (0-100%)

### Photoresistor (Light Sensor)
- **Type:** Analog Input
- **Returns:** Higher value = more light
- **Use Case:** Detect ambient light, auto-brightness
- **Mental Model:** `window.screen.brightness` sensor

### Temperature Sensor (LM35 or DHT11)
- **Type:** Analog Input (LM35) or Digital (DHT11)
- **Returns:** Voltage proportional to temperature
- **Use Case:** Room temperature monitoring
- **Mental Model:** `navigator.temperature` API

### Ultrasonic Sensor (HC-SR04)
- **Type:** Digital I/O (Trigger + Echo pins)
- **Returns:** Distance in cm (2-400 cm range)
- **Use Case:** Proximity detection, obstacle avoidance
- **Mental Model:** Sonar/radar - measures distance
- **How it works:** Sends ultrasonic pulse, measures echo time

### Infrared Receiver
- **Type:** Digital Input
- **Returns:** IR remote control codes
- **Use Case:** Remote control projects
- **Mental Model:** Bluetooth/WiFi receiver for IR signals

---

## 3. OUTPUT COMPONENTS (Actuators - Control Things)

### LEDs (Light Emitting Diodes)
- **Type:** Digital/PWM Output
- **Control:** ON/OFF (digital) or brightness (PWM)
- **Colors:** Red, Green, Blue, Yellow, White
- **Important:** Always use with resistor (220Ω-1kΩ)
- **Mental Model:** Like `console.log()` but visual
- **Use Case:** Status indicators, debugging, displays

### RGB LED
- **Type:** 3 PWM Outputs (one per color)
- **Control:** Mix Red, Green, Blue to create any color
- **Mental Model:** CSS `rgb(r, g, b)` but in hardware

### Buzzer (Passive/Active)
- **Type:** Digital/PWM Output
- **Control:** Tone/frequency for passive, ON/OFF for active
- **Use Case:** Alarms, notifications, music
- **Mental Model:** `audio.play()` but simple beeps

### Servo Motor
- **Type:** PWM Output
- **Control:** Angle (0-180 degrees)
- **Use Case:** Robotic arms, steering, precise positioning
- **Mental Model:** Rotate to specific angle (like CSS `transform: rotate()`)

### DC Motor
- **Type:** PWM Output (usually with motor driver)
- **Control:** Speed and direction
- **Use Case:** Wheels, fans, pumps
- **Mental Model:** Continuous rotation (like a fan)

### Relay Module
- **Type:** Digital Output
- **Control:** ON/OFF for high-power devices
- **Use Case:** Control AC appliances, lights, pumps
- **Mental Model:** Electronic switch for high voltage/current
- **Warning:** Can control mains voltage - be careful!

---

## 4. DISPLAY COMPONENTS (Output - Show Information)

### LCD Display (16x2 or 20x4)
- **Type:** I2C or Parallel Interface
- **Shows:** Text (2-4 lines, 16-20 characters each)
- **Use Case:** Display sensor data, menus, status
- **Mental Model:** Terminal/console output (fixed character grid)

### 7-Segment Display
- **Type:** Digital Output (multiple pins)
- **Shows:** Numbers 0-9
- **Use Case:** Counters, clocks, scoreboards
- **Mental Model:** Digital clock display

### LED Matrix (8x8)
- **Type:** Serial (SPI/I2C) or parallel
- **Shows:** Patterns, animations, icons
- **Use Case:** Scrolling text, games, visualizations
- **Mental Model:** Low-res pixel display

---

## 5. COMMUNICATION COMPONENTS

### Bluetooth Module (HC-05/HC-06)
- **Type:** UART Serial
- **Use Case:** Wireless communication with phone/computer
- **Mental Model:** WebSocket/Bluetooth API

### WiFi Module (ESP8266) - if included
- **Type:** Serial/SPI
- **Use Case:** Internet connectivity, IoT projects
- **Mental Model:** `fetch()` API for hardware

---

## 6. SUPPORTING COMPONENTS

### Breadboard
- **What it is:** Solderless prototyping board
- **Mental Model:** RAM - temporary connections, non-permanent
- **How it works:**
  - Rows are connected horizontally (a-e, f-j)
  - Power rails (+ and -) run vertically along sides

### Jumper Wires
- **What it is:** Connecting wires (Male-Male, Male-Female, Female-Female)
- **Mental Model:** Network cables / function calls between components

### Resistors
- **Values:** 220Ω, 1kΩ, 10kΩ, etc. (color-coded)
- **Use Case:**
  - Limit current (protect LEDs)
  - Pull-up/pull-down for buttons
  - Voltage dividers for sensors
- **Mental Model:** Rate limiting / throttling for electricity

### Capacitors
- **What it is:** Stores electrical charge temporarily
- **Use Case:** Smooth power, filter noise, timing circuits
- **Mental Model:** Battery/cache for quick bursts

### Transistors (NPN/PNP)
- **What it is:** Electronic switch/amplifier
- **Use Case:** Control high-power devices, logic gates
- **Mental Model:** `if` statement in hardware (small signal controls large signal)

---

## 7. CAPABILITY MATRIX

| Component | Input/Output | Data Type | Use Case |
|-----------|-------------|-----------|----------|
| Button | Input | Digital | Trigger actions |
| Potentiometer | Input | Analog | Variable control |
| Light Sensor | Input | Analog | Ambient sensing |
| Temperature | Input | Analog/Digital | Environment monitoring |
| Ultrasonic | Input | Digital | Distance measurement |
| LED | Output | Digital/PWM | Status indication |
| Buzzer | Output | Digital/PWM | Audio alerts |
| Servo | Output | PWM | Precise positioning |
| DC Motor | Output | PWM | Continuous rotation |
| LCD | Output | I2C/Parallel | Text display |
| Relay | Output | Digital | High-power control |

---

## 8. COMMON PROTOCOLS (How Components Talk)

### Digital I/O
- Simple HIGH/LOW (5V/0V)
- Like boolean true/false

### Analog
- Voltage reading (0-5V → 0-1023)
- Like reading float values

### PWM (Pulse Width Modulation)
- Fake analog by fast switching
- Like opacity or volume (0-255)

### I2C (Inter-Integrated Circuit)
- 2-wire serial protocol (SDA, SCL)
- Used by: LCD, sensors, EEPROMs
- Mental Model: USB bus for microcontrollers

### SPI (Serial Peripheral Interface)
- 4-wire high-speed protocol (MOSI, MISO, SCK, SS)
- Used by: SD cards, displays, sensors
- Mental Model: PCIe bus for microcontrollers

### UART/Serial
- 2-wire (TX, RX) communication
- Used by: Bluetooth, GPS, computer communication
- Mental Model: Network socket / stdio

---

## 9. PROJECT COMPLEXITY LEVELS

### Level 1: Single Input/Output (1-2 components)
- Blink LED ✅
- Read button, control LED
- Read potentiometer, control LED brightness

### Level 2: Multi-Component (3-5 components)
- Traffic light (3 LEDs + timing)
- Temperature display (sensor + LCD)
- Distance alarm (ultrasonic + buzzer)

### Level 3: Interactive Systems (5-10 components)
- Weather station (temp + humidity + LCD)
- Robot car (motors + ultrasonic + sensors)
- Home automation (relay + sensors + display)

### Level 4: Communication & IoT (10+ components)
- Bluetooth-controlled robot
- WiFi weather station with web dashboard
- RFID door lock system

---

## 10. LIMITATIONS TO REMEMBER

- **No Operating System:** One program at a time
- **Limited RAM:** 32 KB (be mindful of arrays/strings)
- **No Multithreading:** Everything is sequential (use state machines)
- **Real-Time:** No delay tolerance (avoid `delay()` in complex projects)
- **Power:** 5V/3.3V only (not battery friendly for long term)
- **I/O Limited:** Only ~20 pins (use multiplexers for more)

---

## NEXT: Ready to Experiment!

All components documented. Ready to build experiments in `/r4-exp/`.
