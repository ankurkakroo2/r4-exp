# Arduino Mental Model for Software Engineers

## The Core Philosophy

**Arduino is EVENT-DRIVEN HARDWARE**
- Your code runs in an infinite loop
- You're constantly polling sensors and updating outputs
- Think: Game loop, not web server

---

## 1. THE EXECUTION MODEL

### Traditional Software (Your Mac)
```javascript
// Multi-threaded, event-driven
async function handleRequest() {
  const data = await fetchAPI();
  processData(data);
}

// Multiple programs run simultaneously
// OS handles scheduling
```

### Arduino (Embedded)
```cpp
void setup() {
  // Runs ONCE on boot
  // Like constructor or main()
}

void loop() {
  // Runs FOREVER, as fast as possible
  // Like: while(true) { ... }
  // No OS, no scheduler, just your code

  readSensors();     // Poll inputs
  processData();     // Business logic
  updateOutputs();   // Control actuators
}
```

**Key Insight:**
- Arduino has NO operating system
- ONE program runs in infinite loop
- Loop executes thousands of times per second
- Think of it as a **game engine update loop**

---

## 2. THE I/O MODEL: Software vs Hardware

| Software Concept | Arduino Hardware Equivalent |
|-----------------|----------------------------|
| `console.log()` | LED, LCD, Serial Monitor |
| `stdin.read()` | Button, Sensor |
| `fetch(url)` | Serial, Bluetooth, WiFi module |
| `setTimeout()` | `millis()` based state machine |
| `EventListener` | Polling in `loop()` or Interrupts |
| Database | EEPROM (tiny persistent storage) |
| Boolean variable | Digital Pin (HIGH/LOW) |
| Integer (0-255) | Analog Pin read (0-1023) |
| PWM/Opacity (0-1) | `analogWrite()` (0-255) |

---

## 3. THINKING IN CIRCUITS (The Hardware Stack)

### Software Stack:
```
Application Code
    ↓
Libraries/Frameworks
    ↓
Operating System
    ↓
Hardware Drivers
    ↓
CPU/RAM/Disk
```

### Arduino Stack:
```
Your Sketch (.ino)
    ↓
Arduino Libraries (Servo, LCD, etc.)
    ↓
Arduino Core (digitalWrite, analogRead, etc.)
    ↓
AVR/ARM Hardware Registers
    ↓
Electronic Components (LEDs, Sensors, Motors)
    ↓
Physical World (Light, Motion, Temperature)
```

**The Magic:** Your code DIRECTLY controls physical reality!

---

## 4. THE CIRCUIT MENTAL MODEL

### Software: Data Flow
```
Input → Process → Output
stdin → algorithm → stdout
```

### Hardware: Current Flow
```
Power Source (5V)
    ↓
Arduino Pin (OUTPUT) → Resistor → LED → Ground (0V)
    ↓
Current flows, LED lights up
```

**Ohm's Law (The only physics you need):**
```
Voltage (V) = Current (I) × Resistance (R)

V = I × R

5V = 20mA × 220Ω (safe for LED)
```

**Rule of Thumb:**
- LEDs: Always use 220Ω-1kΩ resistor
- Motors: Use transistor/relay (Arduino pins can't provide enough current)
- Buttons: Use pull-up/pull-down resistor (10kΩ)

---

## 5. TIMING IN ARDUINO (No async/await!)

### ❌ WRONG: Using `delay()` (blocking)
```cpp
void loop() {
  digitalWrite(LED, HIGH);
  delay(1000);              // ❌ BLOCKS for 1 second!
  digitalWrite(LED, LOW);   // Can't read sensors during delay
  delay(1000);
}
```
**Problem:** Can't do anything else during `delay()`

### ✅ CORRECT: Using `millis()` (non-blocking)
```cpp
unsigned long previousMillis = 0;
const long interval = 1000;

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Toggle LED
    digitalWrite(LED, !digitalRead(LED));
  }

  // Can do other things here!
  readSensors();
  checkButtons();
}
```

**Mental Model:** `millis()` is like `Date.now()` - returns time since boot in milliseconds

---

## 6. STATE MACHINES (Your New Best Friend)

Since there's no multi-threading, use **state machines** for complex logic:

```cpp
enum State {
  IDLE,
  WAITING,
  ACTIVE,
  ALARM
};

State currentState = IDLE;

void loop() {
  switch(currentState) {
    case IDLE:
      if (buttonPressed()) {
        currentState = WAITING;
        startTime = millis();
      }
      break;

    case WAITING:
      if (millis() - startTime > 5000) {
        currentState = ACTIVE;
      }
      break;

    case ACTIVE:
      // Do active stuff
      if (conditionMet()) {
        currentState = ALARM;
      }
      break;

    case ALARM:
      triggerAlarm();
      if (resetPressed()) {
        currentState = IDLE;
      }
      break;
  }
}
```

**Mental Model:** State machines = Router/Redux for hardware

---

## 7. DEBUGGING STRATEGIES

### Software Debugging:
- `console.log()`
- Debugger breakpoints
- Stack traces

### Arduino Debugging:
```cpp
void setup() {
  Serial.begin(9600);  // Start serial communication
}

void loop() {
  int sensorValue = analogRead(A0);

  // This is your console.log()!
  Serial.print("Sensor: ");
  Serial.println(sensorValue);

  delay(100);  // Don't spam serial output
}
```

**View output:** Tools → Serial Monitor (in Arduino IDE) or:
```bash
arduino-cli monitor -p /dev/cu.usbmodem2101
```

**Other debugging techniques:**
- Blink LED = poor man's debugger (different patterns = different states)
- LCD display = live variable inspector
- Multimeter = check voltage/current

---

## 8. PROJECT DESIGN PATTERN

### The Mental Checklist:

1. **Define the Goal**
   - What should it do?
   - What inputs do I need?
   - What outputs do I control?

2. **Map Components to Functions**
   - Inputs: Buttons, Sensors → `digitalRead()`, `analogRead()`
   - Outputs: LEDs, Motors, Display → `digitalWrite()`, `analogWrite()`
   - Processing: State machine, threshold checks

3. **Plan the Circuit**
   - Draw connections on paper first
   - Power: 5V + GND to breadboard rails
   - Each component: Pin → Component → Resistor (if needed) → GND

4. **Write Pseudocode**
   ```cpp
   setup:
     configure pins
     initialize components

   loop:
     read sensors
     process logic (state machine)
     update outputs
   ```

5. **Implement Incrementally**
   - Test each component separately first
   - Combine components one by one
   - Debug with Serial.println()

6. **Refine and Optimize**
   - Remove `delay()`, use `millis()`
   - Extract reusable functions
   - Add error handling

---

## 9. COMMON PATTERNS

### Pattern 1: Threshold Detection
```cpp
int sensorValue = analogRead(A0);
if (sensorValue > THRESHOLD) {
  digitalWrite(LED, HIGH);  // Alert!
}
```
**Use Case:** Temperature alarm, light detection

### Pattern 2: Debouncing (for buttons)
```cpp
// Buttons can "bounce" (multiple signals from one press)
int lastState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void loop() {
  int reading = digitalRead(BUTTON);

  if (reading != lastState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      // Button actually pressed
      handleButtonPress();
    }
  }

  lastState = reading;
}
```

### Pattern 3: Averaging (noise reduction)
```cpp
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;

void loop() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(A0);
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;

  int average = total / numReadings;
  // Use average instead of raw reading
}
```

### Pattern 4: Mapping Values
```cpp
// Map sensor range (0-1023) to output range (0-255)
int sensorValue = analogRead(A0);
int brightness = map(sensorValue, 0, 1023, 0, 255);
analogWrite(LED_PIN, brightness);
```

---

## 10. POWER & CURRENT RULES

### What Arduino Pins Can Handle:
- **Voltage:** 5V (digital HIGH), 0V (digital LOW)
- **Current per pin:** Max 20mA (safe: 10-15mA)
- **Total current (all pins):** Max 200mA

### What This Means:
✅ **Direct connection OK:**
- LEDs (with resistor) - ~10-20mA
- Small signal transistors
- Logic-level inputs

❌ **Need external power/driver:**
- Motors (100mA+)
- Relays
- High-power LEDs
- Servos (sometimes)
- Buzzers (some)

**Solution:** Use transistor, MOSFET, or motor driver IC

---

## 11. LIBRARIES: The NPM of Arduino

Arduino has a massive ecosystem of libraries:

```cpp
#include <Servo.h>        // Control servo motors
#include <LiquidCrystal.h> // LCD displays
#include <DHT.h>          // Temperature/humidity sensors
#include <Wire.h>         // I2C communication
#include <SPI.h>          // SPI communication
```

**Install libraries:**
```bash
arduino-cli lib search <name>
arduino-cli lib install "<library name>"
```

**Or in IDE:** Tools → Manage Libraries

---

## 12. THE LEARNING CURVE

```
Week 1: "Wow, I made LED blink!"
Week 2: "I can read sensors and display data!"
Week 3: "State machines make sense now"
Week 4: "I built a functional project!"
Month 2: "I understand circuits and can design systems"
Month 3: "I'm combining multiple sensors and actuators"
```

**Key Milestones:**
1. ✅ Blink LED (you're here!)
2. Button controls LED
3. Sensor displays on LCD
4. Multi-component project (e.g., temperature alarm)
5. Communication (Bluetooth/Serial)
6. Advanced project (robot, IoT device)

---

## 13. WHEN YOU'RE STUCK

### Hardware Not Working?
1. Check power (5V, GND connected?)
2. Check connections (loose wires?)
3. Test component separately
4. Use multimeter (voltage on pins?)
5. Check polarity (LED, capacitor, IC orientation)

### Code Not Working?
1. Serial.println() everything
2. Test functions independently
3. Simplify to minimal example
4. Check pinMode() configuration
5. Verify pin numbers in code match physical wiring

### Component Damaged?
- Smoke = probably dead
- Very hot = probably wrong connection
- LEDs backward = won't work (but usually not damaged)
- Arduino survives most mistakes (has protection)

---

## 14. THE MINDSET SHIFT

**From Software to Hardware:**

| Software Thinking | Hardware Thinking |
|------------------|-------------------|
| "Will this compile?" | "Will this fry my components?" |
| "Is this efficient?" | "Does this draw too much current?" |
| "Async/await for concurrency" | "State machines for multi-tasking" |
| "Database for persistence" | "EEPROM (tiny, limited writes)" |
| "Cloud APIs" | "Serial/I2C/SPI protocols" |
| "Logging to files" | "Serial Monitor or LCD" |
| "Try/catch errors" | "Check sensor values, add timeouts" |

**But Also:**
- Same debugging skills apply (isolate, test, verify)
- Same modular design (functions, abstraction)
- Same iterative development (prototype, test, refine)

---

## 15. YOUR SUPERPOWER

**As a software engineer, you already have:**
- Logic and algorithms ✅
- Debugging mindset ✅
- Problem decomposition ✅
- Reading documentation ✅

**You're just learning:**
- Basic electronics (Ohm's law, current, voltage)
- Circuit design (breadboarding)
- Component behavior (datasheets)
- Physical debugging (multimeter, visual inspection)

**You'll be dangerous (in a good way) very quickly!**

---

## READY TO BUILD!

You now have the mental model. Let's start experimenting!

**Suggested experiment progression in `/r4-exp/`:**
1. `01-button-led/` - Digital input/output
2. `02-potentiometer-led/` - Analog input, PWM output
3. `03-sensor-display/` - Read sensor, show on LCD
4. `04-state-machine/` - Traffic light or sequence
5. `05-integrated/` - Combine multiple components

**Pick your first experiment or tell me what interests you!**
