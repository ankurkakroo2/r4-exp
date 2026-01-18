# Arduino R4 Experiments 🤖

A software engineer's journey into the world of Arduino, hardware, and embedded systems. This repository documents hands-on experiments with the Arduino Uno R4, exploring sensors, displays, motors, and sound while building practical and fun projects.

## 🎯 Project Goals

- **Learn by doing**: Transition from pure software to hardware/software integration
- **Understand fundamentals**: I/O, sensors, actuators, communication protocols
- **Build incrementally**: Start simple, add complexity progressively
- **Document thoroughly**: Each experiment is self-contained with detailed explanations
- **Have fun**: Create interactive, engaging projects that respond to the world

## 🛠️ Hardware Setup

**Board:** Arduino Uno R4 Minima (R4-lite clone)
**Kit:** Quad Store Super Starter Kit for Arduino

**Key Components:**
- HC-SR04 Ultrasonic Distance Sensor
- 16x2 LCD Display (I2C)
- SG90 Servo Motor
- Passive Buzzer
- Active Buzzer
- Various LEDs, resistors, sensors
- Breadboard for prototyping

**Software:**
- Arduino IDE 2.3.7
- Arduino CLI for command-line workflows
- Core: `arduino:renesas_uno` (R4 support)

## 📚 Experiments

### Experiment 01: Blink
**Concepts:** Basic I/O, Digital output, LED control
**Status:** ✅ Completed
**Description:** The classic "Hello World" of Arduino - blink the onboard LED.

**Key Learnings:**
- Arduino execution model (setup + loop)
- Digital output with `digitalWrite()`
- Timing with `delay()`
- Uploading and running code on hardware

---

### Experiment 02: LCD Display - "Karishma, i love you"
**Concepts:** I2C communication, LCD control, Text display
**Status:** ✅ Completed
**Hardware:** 16x2 LCD with I2C backpack
**Location:** `experiments/02-lcd-display/`

**What it does:**
- Displays custom message on 16x2 LCD
- Uses I2C protocol (2-wire communication)
- Address detection with I2C scanner

**Key Learnings:**
- I2C protocol (SDA/SCL pins)
- Device addressing (0x27)
- LiquidCrystal_I2C library
- Multi-device bus communication
- Troubleshooting with I2C scanner

**Files:**
- `DisplayName/DisplayName.ino` - Main display code
- `I2C_Scanner/I2C_Scanner.ino` - Address detection utility
- `WIRING.txt` - Detailed wiring guide
- `README.md` - I2C protocol explanation

---

### Experiment 03: Super Mario Bros - Music + Animation
**Concepts:** Custom LCD characters, Tone generation, Synchronization
**Status:** ✅ Completed
**Hardware:** 16x2 LCD (I2C), Passive buzzer
**Location:** `experiments/03-mario-music-animation/`

**What it does:**
- Plays Super Mario Bros theme on buzzer
- Shows Mario walking across LCD screen
- Custom 5×8 pixel character animation
- Synchronized music and movement

**Key Learnings:**
- Custom LCD characters with binary notation (`0b01110`)
- `tone()` function for music playback
- Musical note frequencies (A4 = 440Hz)
- Frame-by-frame animation
- Passive vs Active buzzers
- Multi-tasking with sequential execution

**Files:**
- `MarioGame/MarioGame.ino` - Complete game code
- `WIRING.txt` - Buzzer connection guide
- `README.md` - Music theory and animation concepts

**Cool Features:**
- 48-note melody array
- 2-frame walking animation
- Victory jingle at end
- Continuous looping

---

### Experiment 04: Proximity Alarm - Parking Sensor
**Concepts:** Ultrasonic sensing, Distance measurement, Zone-based alerts
**Status:** ✅ Completed
**Hardware:** HC-SR04 sensor, Buzzer, LCD
**Location:** `experiments/04-proximity-alarm/`

**What it does:**
- Measures distance using ultrasonic sensor (like sonar)
- Beeps faster as objects get closer
- Shows real-time distance and status on LCD
- Works like car parking sensors

**Key Learnings:**
- Ultrasonic sensor physics (speed of sound = 343 m/s)
- `pulseIn()` for pulse width measurement
- Distance calculation: `distance = (duration × 0.034) / 2`
- `map()` function for range scaling
- Non-blocking timing with `millis()`
- Zone-based state machines

**Distance Zones:**
- >200cm: Silent, safe
- 100-200cm: Slow beeps (1/sec)
- 50-100cm: Medium beeps (2-3/sec)
- 20-50cm: Fast beeps (5+/sec)
- <15cm: Continuous alarm!

**Files:**
- `ProximityAlarm/ProximityAlarm.ino` - Main sensor code
- `WIRING.txt` - HC-SR04 wiring guide
- `README.md` - Ultrasonic physics explanation

---

### Experiment 05: Proximity Fan - Servo + Variable Tempo Music
**Concepts:** Servo control, PWM, Variable tempo playback, Multi-parameter adaptation
**Status:** ✅ Completed
**Hardware:** HC-SR04 sensor, SG90 servo, Buzzer, LCD
**Location:** `experiments/05-proximity-fan/`

**What it does:**
- Measures distance with ultrasonic sensor
- Oscillates servo motor faster as objects approach (fan effect)
- Plays Mario theme at variable tempo (faster when closer)
- Shows distance, tempo %, and servo speed on LCD

**Key Learnings:**
- Servo motor control with Servo library
- PWM signal generation (50Hz, 0.5-2.5ms pulse width)
- Angle positioning (0-180°)
- Creating oscillation by toggling positions
- Variable tempo music playback
- Non-blocking multi-tasking (distance sensing + music + servo + LCD)
- Tempo vs Pitch (speed vs frequency)
- Percentage-based scaling (100% = normal, 200% = 2x faster)

**Adaptive Behavior:**
```
Distance    Tempo    Servo Speed    Experience
>150cm      Silent   Stopped        Safe, quiet
100cm       100%     450ms          Normal Mario
50cm        200%     300ms          2x faster music!
20cm        275%     175ms          Super fast
<15cm       300%     150ms          3x speed MAX!
```

**Files:**
- `ProximityFan/ProximityFan.ino` - Main adaptive system code
- `ServoTest/ServoTest.ino` - Servo power diagnostic tool
- `WIRING.txt` - Complete wiring guide (sensor + servo + buzzer + LCD)
- `README.md` - Servo mechanics and PWM explanation

**Technical Highlights:**
- Non-blocking music player with melody looping
- Formula: `adjustedDuration = (baseDuration × 100) / tempoMultiplier`
- 48-note Mario melody array
- Simultaneous control of 3 outputs from 1 input (distance)
- Smooth tempo transitions

---

## 🧠 Mental Model

As a software engineer, here's how I think about Arduino:

**Execution Model:**
```cpp
void setup() {
  // Runs ONCE at startup
  // Like constructor / initialization
}

void loop() {
  // Runs FOREVER in infinite loop
  // Like game loop or event loop
  // ~16,000 times per second!
}
```

**Key Differences from Traditional Software:**
- **No OS**: Direct hardware control
- **Real-time**: Timing matters (use `millis()` not `delay()`)
- **Limited resources**: 256KB flash, 32KB RAM
- **Physical I/O**: Sensors, motors, displays are your interface
- **State machines**: Track state across loop iterations

**Non-Blocking Pattern** (Critical!):
```cpp
// BAD - Blocks everything
delay(1000);

// GOOD - Non-blocking
unsigned long lastTime = 0;
if (millis() - lastTime > 1000) {
  // Do something
  lastTime = millis();
}
```

See `MENTAL_MODEL.md` for detailed explanations.

---

## 🚀 Future Experiment Ideas

### 🎮 Immediate Ideas
- [ ] **RGB LED Mood Light**: Distance-based color changes
- [ ] **Theremin**: Distance controls pitch (musical instrument)
- [ ] **Multi-sensor Array**: 360° obstacle detection
- [ ] **Data Logger**: Log sensor data over time, plot on Serial Plotter
- [ ] **Stepper Motor Control**: Precise positioning vs servo

### 🌟 Intermediate Ideas
- [ ] **Temperature Alarm**: DHT11 sensor with threshold alerts
- [ ] **Light-following Robot**: Photoresistor + dual servos
- [ ] **MIDI Controller**: Turn sensors into music controller
- [ ] **Web Dashboard**: ESP32 WiFi + sensor data visualization
- [ ] **Gesture Control**: Multiple ultrasonic sensors for hand tracking

### 🔥 Advanced Ideas
- [ ] **Mini Radar Display**: Rotating ultrasonic + processing visualization
- [ ] **Persistence of Vision**: LED strip + motor = mid-air display
- [ ] **Multi-room Sensor Network**: I2C/SPI multi-board communication
- [ ] **Computer Vision**: Camera module + object detection
- [ ] **Custom PCB**: Design and fabricate custom shield

### 💡 Creative Applications
- [ ] **Interactive Art Installation**: Proximity-reactive sound/light sculpture
- [ ] **Plant Monitor**: Soil moisture + auto-watering system
- [ ] **Pet Feeder**: Scheduled feeding with ultrasonic level sensor
- [ ] **Bike Speedometer**: Hall effect sensor + LCD display
- [ ] **Door/Window Alert**: Magnetic switches + notification system

---

## 📊 Progress Tracker

| Experiment | Concept | Hardware | Status |
|------------|---------|----------|--------|
| 01 - Blink | Digital I/O | Onboard LED | ✅ |
| 02 - LCD Display | I2C, Text | LCD 16x2 | ✅ |
| 03 - Mario Game | Animation, Sound | LCD, Buzzer | ✅ |
| 04 - Proximity Alarm | Ultrasonic, Zones | HC-SR04, Buzzer | ✅ |
| 05 - Proximity Fan | Servo, Variable Tempo | HC-SR04, Servo, Buzzer | ✅ |

**Sensors Used:** 2/6 (Ultrasonic, LCD)
**Displays Used:** 1/4 (16x2 LCD)
**Motors Used:** 1/2 (SG90 Servo)
**Components Explored:** ~15%

Lots more to explore! 🚀

---

## 🔧 Development Workflow

### Quick Commands

```bash
# Compile sketch
arduino-cli compile --fqbn arduino:renesas_uno:minima

# Upload to board
arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:renesas_uno:minima

# Install library
arduino-cli lib install <LibraryName>

# Monitor serial output
arduino-cli monitor -p /dev/cu.usbmodem2101 -c baudrate=9600
```

### Project Structure

```
r4-exp/
├── experiments/
│   ├── 01-blink/
│   ├── 02-lcd-display/
│   ├── 03-mario-music-animation/
│   ├── 04-proximity-alarm/
│   └── 05-proximity-fan/
├── KIT_COMPONENTS.md       # Full component inventory
├── MENTAL_MODEL.md         # Thinking framework for Arduino
├── QUICK_REFERENCE.md      # Syntax cheat sheet
└── README.md               # This file
```

Each experiment folder contains:
- `<ExperimentName>.ino` - Arduino sketch
- `WIRING.txt` - Detailed wiring diagrams
- `README.md` - Concepts, theory, and explanations

---

## 🎓 Key Learnings

### Technical Skills
- ✅ I2C communication protocol
- ✅ Ultrasonic distance measurement
- ✅ PWM for servo control
- ✅ Tone generation and music playback
- ✅ Custom LCD character creation
- ✅ Non-blocking multi-tasking patterns
- ✅ State machine design
- ✅ Real-time sensor processing

### Hardware Understanding
- ✅ Voltage levels (5V logic, 3.3V compatibility)
- ✅ Current limits (servo power requirements)
- ✅ Pull-up/pull-down resistors
- ✅ Breadboard power distribution
- ✅ Common ground requirements
- ✅ PWM vs Digital vs Analog pins

### Problem Solving
- ✅ I2C address debugging with scanners
- ✅ Power supply troubleshooting
- ✅ Servo timing optimization
- ✅ Non-blocking algorithm design
- ✅ Multi-parameter adaptive systems

---

## 🌱 Philosophy

> "The best way to learn is to build something that makes you smile."

Every experiment here:
- Solves a real problem or creates something fun
- Teaches at least one new concept
- Builds on previous knowledge
- Is thoroughly documented for future reference
- Actually works (no "in theory" code!)

This isn't about building the most complex thing possible - it's about understanding **how things work** from first principles, and having fun along the way.

---

## 📖 Resources

**Official Documentation:**
- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Arduino Uno R4 Minima Docs](https://docs.arduino.cc/hardware/uno-r4-minima/)

**Libraries Used:**
- `LiquidCrystal_I2C` - LCD display control
- `Servo` - Servo motor control
- `Wire` - I2C communication (built-in)

**Learning Resources:**
- Each experiment's README.md has detailed concept explanations
- `MENTAL_MODEL.md` - Software engineer's guide to Arduino
- Inline code comments explain the "why" not just the "what"

---

## 🤝 Contributing

This is a personal learning repository, but ideas and suggestions are welcome! If you have:
- Cool experiment ideas that build on existing concepts
- Better explanations for complex topics
- Interesting hardware suggestions from the kit

Feel free to open an issue or PR!

---

## 📝 License

MIT License - Feel free to use this code and documentation for your own learning!

---

## 🙏 Acknowledgments

- **Arduino Community**: For extensive documentation and examples
- **Claude (Anthropic)**: AI pair programmer helping me learn hardware
- **Open Source Libraries**: Making complex hardware accessible

---

**Current Status:** 5 experiments completed, many more ideas brewing! 🔧⚡

**Last Updated:** January 2026

---

*From `console.log()` to `digitalWrite()` - the journey continues!* 🚀
