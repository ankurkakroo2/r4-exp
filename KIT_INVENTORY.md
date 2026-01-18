# Your Arduino Kit - Complete Component Inventory

**Kit:** Quad Store Super Starter Kit for Uno R3/R4

---

## 🎛️ SENSORS (Input Devices)

### Environmental Sensors
- **DHT11 Temperature & Humidity Sensor** (4-pin)
  - Measures: Temperature (0-50°C) and Humidity (20-90%)
  - Use for: Weather station, climate control, environmental monitoring

- **Photoresistor (LDR - Light Dependent Resistor)**
  - Detects: Light intensity
  - Use for: Auto-brightness, light-activated projects, day/night detection

### Distance & Motion
- **Ultrasonic Sensor HC-SR04**
  - Measures: Distance (2cm - 4m)
  - Use for: Obstacle avoidance, parking sensor, distance meter

- **Vibration Switch**
  - Detects: Vibration/shock
  - Use for: Alarm systems, shake detection, impact sensors

### Detection
- **Flame/Fire Sensor**
  - Detects: Infrared light from flames
  - Use for: Fire alarm, flame detection projects

- **IR Receiver**
  - Receives: Infrared signals from remote
  - Use for: Remote control projects, wireless communication
  - **IR Remote** included (battery not included)

---

## 📟 DISPLAYS (Output Devices)

### Text Display
- **16x2 LCD Display with I2C Module** ✅ (You're using this one!)
  - Shows: 2 rows × 16 characters
  - Use for: Text messages, sensor data, menus, status displays

### Number Displays
- **7-Segment Display (1-digit)**
  - Shows: Single digit 0-9
  - Use for: Counters, timers, single value display

- **7-Segment Display (4-digit)**
  - Shows: Four digits 0-9
  - Use for: Clock, counter, temperature display, score display

### Graphical Display
- **8×8 Dot Matrix LED Display**
  - Shows: Patterns, animations, simple graphics
  - Use for: Scrolling text, icons, simple games, animations

---

## ⚙️ MOTORS & ACTUATORS (Movement)

### Servo Motor
- **SG90 Servo Motor**
  - Rotation: 0-180 degrees (precise positioning)
  - Use for: Robot arms, pan-tilt camera, steering mechanisms

### Stepper Motor
- **28BYJ-48 Stepper Motor (5V)**
  - Rotation: Precise step-by-step movement
  - **ULN2003 Stepper Driver Board** (required to control it)
  - Use for: Precise positioning, CNC, 3D printer movements, robotics

---

## 💡 LEDs & LIGHTING

### Individual LEDs
- **Red LEDs**
- **Green LEDs**
- **Blue LEDs**
  - Use for: Status indicators, debugging, decorative lighting

### RGB Module
- **RGB LED Module**
  - Colors: Can display any color (mix Red, Green, Blue)
  - Use for: Mood lighting, color indicators, visual effects

---

## 🔊 AUDIO OUTPUT

- **Active Buzzer**
  - Makes: Fixed tone when powered
  - Use for: Simple beeps, alarms, notifications

- **Passive Buzzer**
  - Makes: Different tones/melodies (frequency controlled)
  - Use for: Music, ringtones, multi-tone alarms

---

## 🔌 INTEGRATED CIRCUITS & MODULES

### Microcontroller
- **Arduino Uno R4 Compatible Board** (Your R4-lite!)
  - The brain of all your projects
  - Already set up and working ✅

### Driver ICs
- **ULN2003 Stepper Motor Driver**
  - Drives: 28BYJ-48 stepper motor
  - Amplifies: Arduino signals to control motor

### Logic ICs
- **IC 74HC595 (Shift Register)**
  - Function: Expands output pins
  - Converts: 3 pins → 8 output pins
  - Use for: Controlling many LEDs with few pins, multiplexing

---

## 🎚️ CONTROL COMPONENTS

- **Potentiometer 10K**
  - Type: Variable resistor / rotary knob
  - Use for: Volume control, brightness adjustment, user input

---

## 🔋 POWER SUPPLY

- **9V Battery Holder**
  - Includes: Plug connector for Arduino
  - Use for: Portable/standalone projects (battery not included)

- **Breadboard Power Supply Module**
  - Converts: External power → 5V and 3.3V rails for breadboard
  - Use for: Powering sensors and components from external source

---

## 🧰 PASSIVE COMPONENTS

### Resistors (Various Values)
- **220Ω resistors** - For LEDs (current limiting)
- **230Ω resistors** - For LEDs
- **1KΩ resistors** - Pull-up/pull-down, general purpose
- **10KΩ resistors** - Pull-up/pull-down, voltage dividers

### Other
- **40-pin Header** - For connections and extensions

---

## 📊 COMPONENT SUMMARY BY CATEGORY

| Category | Count | Examples |
|----------|-------|----------|
| **Sensors** | 6 | DHT11, Ultrasonic, Flame, Vibration, Photoresistor, IR |
| **Displays** | 4 | LCD 16x2, 7-seg (1 & 4 digit), 8x8 Matrix |
| **Motors** | 2 | Servo, Stepper (+ driver) |
| **LEDs** | 3+ types | Red, Green, Blue, RGB Module |
| **Audio** | 2 | Active Buzzer, Passive Buzzer |
| **ICs** | 2 | 74HC595, ULN2003 |
| **Controls** | 1 | Potentiometer |
| **Power** | 2 | 9V holder, Breadboard PSU |

---

## 🎯 PROJECT IDEAS BY COMPONENT

### Using DHT11 (Temp/Humidity)
- Weather station with LCD display
- Home climate monitor
- Greenhouse automation

### Using Ultrasonic Sensor
- Distance meter with display
- Parking assistant
- Object detector with buzzer alarm
- Robot obstacle avoidance

### Using Flame Sensor
- Fire alarm system
- Automatic fire extinguisher trigger
- Safety monitoring system

### Using Servo Motor
- Pan-tilt camera mount
- Robotic arm
- Door lock mechanism
- Automated window opener

### Using Stepper Motor
- Clock mechanism
- Camera slider
- Drawing machine/plotter
- Automated blinds

### Using 7-Segment Display
- Digital clock
- Score counter
- Temperature display
- Timer/stopwatch

### Using 8x8 Matrix
- Scrolling message display
- Simple games (Snake, Pong)
- Animated icons
- Sound visualizer

### Using RGB LED
- Mood lamp
- Color-changing night light
- Temperature indicator (blue=cold, red=hot)
- Music visualizer

### Using IR Remote
- Home automation controller
- TV remote projects
- Robot remote control
- Distance-controlled lights

### Using Buzzers
- Alarm clock
- Doorbell
- Game sound effects
- Musical instrument
- Morse code trainer

### Using Shift Register (74HC595)
- LED cube controller
- Multiple LED control
- 7-segment display driver
- Expand Arduino pins

---

## 🚀 SUGGESTED LEARNING PATH

### Beginner (Week 1-2)
1. ✅ Blink LED (Done!)
2. ✅ LCD Display (Done!)
3. Button + LED
4. Potentiometer + LED (PWM)
5. Buzzer melodies

### Intermediate (Week 3-4)
6. Temperature display (DHT11 + LCD)
7. Distance meter (Ultrasonic + LCD)
8. Servo control with potentiometer
9. 7-segment counter
10. IR remote control

### Advanced (Week 5-8)
11. Weather station (DHT11 + LCD + multiple displays)
12. Parking sensor (Ultrasonic + buzzer + LEDs)
13. Fire alarm system (Flame sensor + buzzer + display)
14. Stepper motor projects
15. RGB mood lamp with remote control
16. Matrix display animations
17. Multi-component integrated projects

---

## 💡 WHAT YOU CAN BUILD RIGHT NOW

With the components you have, you can build:

✅ **Working Projects:**
- Smart weather station
- Distance measuring tool
- Fire detection alarm
- Remote-controlled RGB lights
- Digital clock
- Temperature/humidity monitor
- Servo-controlled camera mount
- Light-activated system
- Music player (buzzer)
- Interactive displays

---

## 🔗 Component Datasheets

Most components have datasheets online. Search for:
- "DHT11 datasheet"
- "HC-SR04 datasheet"
- "74HC595 datasheet"
- "SG90 servo datasheet"
- "28BYJ-48 stepper motor datasheet"

These contain:
- Pin diagrams
- Electrical specifications
- Timing diagrams
- Example circuits
- Sample code

---

## 📚 Next Steps

You now have an inventory of **25+ unique electronic components**!

**Recommended next experiments:**
1. **Temperature Display** - DHT11 + LCD (show temp and humidity)
2. **Distance Meter** - Ultrasonic sensor + LCD
3. **Remote Control** - IR receiver + LEDs/Buzzer
4. **Servo Control** - Potentiometer controls servo angle

**Which one interests you?**

---

*Source: [Quad Store Arduino Starter Kit](https://quadstore.in/shop/arduino-kits/arduino-starter-kit-arduino-uno-r3-beginners-kit-best-india-online/)*
