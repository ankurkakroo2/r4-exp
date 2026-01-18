# Experiment 05: Proximity Fan - Servo Motor Speed Control

## What We're Building

A **proximity-activated fan system** that:
1. 📏 Measures distance using ultrasonic sensor
2. 🔊 Plays higher pitch sounds as objects get closer
3. 🌀 Spins servo faster as objects approach
4. 📺 Shows real-time distance, pitch, and speed on LCD

**Behavior:**
- **Far away (>150cm):** Silent, servo stopped
- **Medium distance (100-150cm):** Low pitch, slow oscillation
- **Close (50-100cm):** Medium pitch, medium speed
- **Very close (<50cm):** High pitch, fast oscillation
- **Alarm zone (<20cm):** Highest pitch, maximum speed

---

## 🎓 New Concepts You'll Learn

### 1. **Servo Motors - How They Work**

**What is a Servo Motor?**

A servo motor is a special type of motor that can rotate to a **specific angle** (0° to 180°) and **hold that position**.

```
Regular Motor:        Servo Motor:
  Spins continuously    Rotates to exact angles
  ═════════>            0° ──── 90° ──── 180°
  Speed control only    Position control!
```

**Inside a Servo:**
```
┌─────────────────────────────────┐
│  Servo Motor (SG90)             │
│                                 │
│  ┌──────────┐   ┌──────────┐  │
│  │ DC Motor │→→→│ Gearbox  │  │  ← Reduces speed,
│  └──────────┘   └──────────┘  │     increases torque
│                       ↓         │
│                  ┌──────────┐  │
│                  │ Position │  │  ← Potentiometer
│                  │ Sensor   │  │     tells current angle
│                  └──────────┘  │
│                       ↓         │
│                  ┌──────────┐  │
│                  │ Control  │  │  ← Compares target
│                  │ Circuit  │  │     vs actual angle
│                  └──────────┘  │
└─────────────────────────────────┘

Process:
1. You send target angle (90°)
2. Control circuit compares with current position
3. Motor turns until potentiometer reads 90°
4. Motor stops and holds position
```

**Servo Specifications (SG90):**
- **Rotation Range:** 0° to 180° (half circle)
- **Torque:** 1.8 kg·cm (can lift ~1.8 kg at 1 cm distance)
- **Speed:** 0.1 sec/60° (very fast positioning)
- **Voltage:** 4.8V - 6V
- **Control:** PWM signal (pulse width modulation)

---

### 2. **PWM Control - Pulse Width Modulation**

**How do we tell the servo which angle to go to?**

Answer: **PWM signals** with different pulse widths!

```
PWM Signal Structure:
─────┐     ┌──────────────────────────────
     │     │
     └─────┘
     ←──→
   Pulse Width

Period: 20ms (50Hz)

Pulse Width → Angle Mapping:
─────┐  ┌───────────────────  0.5ms (500μs)  = 0°
     └──┘
─────┐    ┌─────────────────  1.0ms (1000μs) = 45°
     └────┘
─────┐      ┌───────────────  1.5ms (1500μs) = 90°
     └──────┘
─────┐        ┌─────────────  2.0ms (2000μs) = 135°
     └────────┘
─────┐          ┌───────────  2.5ms (2500μs) = 180°
     └──────────┘
```

**Arduino Servo Library:**
```cpp
#include <Servo.h>

Servo myServo;
myServo.attach(11);      // Connect to pin 11
myServo.write(90);       // Move to 90° (center)
myServo.write(0);        // Move to 0° (left)
myServo.write(180);      // Move to 180° (right)
```

The library handles all PWM timing for you!

---

### 3. **Creating Oscillation/Fan Effect**

Since the SG90 servo only rotates 0-180° (not continuous), we create a **fan effect** by **rapidly oscillating** back and forth:

```
Slow Oscillation (Far):
  0° ─────→ 180° ─────→ 0° ─────→ 180°
      1 sec      1 sec      1 sec

  Like a slow wave 🌊

Fast Oscillation (Close):
  0° ──→ 180° ──→ 0° ──→ 180° ──→ 0°
     0.1s    0.1s    0.1s    0.1s

  Like a fan 🌀 (appears continuous!)
```

**Implementation Pattern:**
```cpp
void oscillateServo(int speed) {
  // speed = delay between movements (lower = faster)

  servo.write(0);        // Swing to left
  delay(speed);

  servo.write(180);      // Swing to right
  delay(speed);
}

// Usage:
oscillateServo(500);  // Slow (500ms per swing)
oscillateServo(50);   // Fast (50ms per swing) - appears like fan!
```

---

### 4. **Distance to Speed Mapping**

We use the `map()` function to convert distance to servo speed:

```cpp
// Map distance (200cm → 20cm) to speed (slow → fast)
int servoSpeed = map(distance, 20, 200, 50, 500);
//                   ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
//                   input     input     output
//                   value     range     range

// When distance = 200cm → servoSpeed = 500ms (slow)
// When distance = 20cm  → servoSpeed = 50ms (fast)
// Auto-scales in between!
```

Similarly for buzzer pitch:
```cpp
// Map distance (20cm → 200cm) to frequency (high → low)
int pitch = map(distance, 20, 200, 2000, 500);
//          ^^^^^^^^  ^^^^^^^^  ^^^^^^^^^^
//          Closer    Range     Higher pitch
```

---

## 🔌 Components Needed

### From Your Kit:
1. ✅ Arduino Uno R4
2. ✅ HC-SR04 Ultrasonic Sensor (from Experiment 04)
3. ✅ SG90 Servo Motor (NEW!)
4. ✅ Buzzer (from previous experiments)
5. ✅ 16x2 LCD with I2C (from Experiment 02)
6. ✅ Jumper wires

**Optional Enhancement:**
- Attach small paper fan blades to servo horn for visual effect

---

## 🔧 Wiring

### Complete Connection Table:

```
Component Pin         →    Arduino Pin
──────────────────────────────────────────────
HC-SR04 VCC          →    5V
HC-SR04 TRIG         →    Pin 9
HC-SR04 ECHO         →    Pin 10
HC-SR04 GND          →    GND

Servo (Brown/Black)  →    GND
Servo (Red)          →    5V
Servo (Orange/Yellow)→    Pin 11

Buzzer (+)           →    Pin 8
Buzzer (-)           →    GND

LCD GND              →    GND
LCD VCC              →    5V
LCD SDA              →    A4
LCD SCL              →    A5
```

### Servo Wire Colors:

Different manufacturers use different colors:

```
Standard 1:          Standard 2:
Brown  = GND         Black  = GND
Red    = 5V          Red    = 5V
Orange = Signal      White  = Signal

Your SG90 likely has: Brown, Red, Orange
```

---

## 💡 How the Code Works

### Main Algorithm:

```
Loop continuously:
  ├─ 1. Measure distance with ultrasonic sensor
  ├─ 2. Calculate servo oscillation speed from distance
  ├─ 3. Calculate buzzer pitch from distance
  ├─ 4. Oscillate servo (fast if close, slow if far)
  ├─ 5. Play buzzer tone at calculated pitch
  ├─ 6. Update LCD with distance/speed/pitch
  └─ 7. Repeat
```

### Distance Zones:

```cpp
Zone 1: > 150cm  → Silent, servo stopped
Zone 2: 100-150cm → Low pitch (500Hz), slow (400ms)
Zone 3: 50-100cm  → Medium pitch (1000Hz), medium (200ms)
Zone 4: 20-50cm   → High pitch (1500Hz), fast (100ms)
Zone 5: < 20cm    → Max pitch (2000Hz), max speed (50ms)
```

### Non-Blocking Servo Control:

Instead of using `delay()` which blocks everything:

```cpp
// Bad (blocking):
void loop() {
  servo.write(0);
  delay(500);      // Can't measure distance during this!
  servo.write(180);
  delay(500);
}

// Good (non-blocking):
unsigned long lastMove = 0;
int position = 0;

void loop() {
  // Check if it's time to move
  if (millis() - lastMove > servoSpeed) {
    position = (position == 0) ? 180 : 0;  // Toggle
    servo.write(position);
    lastMove = millis();
  }

  // Can measure distance continuously!
  distance = measureDistance();
}
```

---

## 🎯 Expected Behavior

### Test Sequence:

**1. Nothing nearby (>150cm):**
```
LCD Row 0: "Dist: 200cm"
LCD Row 1: "SAFE - Stopped"
Buzzer: Silent
Servo: Stopped at 90° (center)
```

**2. Medium distance (100cm):**
```
LCD Row 0: "Dist: 100cm"
LCD Row 1: "Pitch:800 S:300"
Buzzer: Low beep (800Hz)
Servo: Slow oscillation (300ms per swing)
```

**3. Getting closer (50cm):**
```
LCD Row 0: "Dist: 50cm"
LCD Row 1: "Pitch:1200 S:150"
Buzzer: Medium beep (1200Hz)
Servo: Medium speed (150ms per swing)
```

**4. Very close (20cm):**
```
LCD Row 0: "Dist: 20cm"
LCD Row 1: "Pitch:1800 S:60"
Buzzer: High beep (1800Hz)
Servo: Fast oscillation (60ms) - looks like fan!
```

**5. Alarm zone (<15cm):**
```
LCD Row 0: "Dist: 10cm ALARM"
LCD Row 1: "MAX SPEED!!!"
Buzzer: Continuous high tone (2000Hz)
Servo: Maximum speed (50ms) - rapid fan motion
```

---

## 🔬 Physics & Engineering Concepts

### 1. **Torque vs Speed Trade-off**

```
Servo Internal Gearbox:

DC Motor (fast, weak)
    ↓
  [Gear 1:10]  ← Reduces speed by 10×
    ↓             Increases torque by 10×
  [Gear 1:5]   ← Further reduction
    ↓
Output Shaft (slow, strong)

Final ratio: ~1:50
  Motor: 6000 RPM, 0.01 kg·cm torque
  Output: 120 RPM, 1.8 kg·cm torque
```

### 2. **PWM Frequency Matters**

```
Servo needs 50Hz PWM (20ms period):
  Too fast (100Hz):  Servo jitters, overheats
  Too slow (25Hz):   Servo lags, imprecise
  Just right (50Hz): Smooth, accurate
```

### 3. **Power Considerations**

```
SG90 Current Draw:
  Idle (holding position): ~10mA
  Moving (no load):        ~100mA
  Stalled (max torque):    ~600mA

Arduino 5V pin limit: 500mA total

⚠️  Single servo OK from 5V pin
⚠️  Multiple servos need external power supply!
```

---

## 🎨 Code Features

### 1. **Adaptive Speed Control**
```cpp
// Servo speed adapts to distance in real-time
servoSpeed = map(distance, 15, 150, 50, 500);
```

### 2. **Pitch Variation**
```cpp
// Buzzer pitch changes with distance
int pitch = map(distance, 15, 150, 2000, 500);
tone(buzzerPin, pitch);
```

### 3. **Multi-Zone Detection**
```cpp
5 distinct zones with different behaviors
```

### 4. **LCD Real-time Feedback**
```cpp
Shows distance, pitch frequency, and servo speed
```

### 5. **Non-Blocking Timing**
```cpp
Uses millis() for smooth multi-tasking
```

---

## 🚀 Real-World Applications

This principle is used in:

1. **Automated Cooling Systems**
   - Server racks: fan speed based on temperature
   - Laptops: thermal management

2. **Warning Systems**
   - Construction vehicles: reverse warning with visual flags
   - Security systems: escalating alerts

3. **Interactive Art**
   - Kinetic sculptures respond to viewers
   - Museum exhibits react to proximity

4. **Robotics**
   - Obstacle avoidance with graded response
   - Adaptive speed control near objects

5. **HVAC Systems**
   - Variable speed fans based on occupancy
   - Smart thermostats

---

## 🔧 Calibration & Tuning

### Adjust Servo Speed Range:

```cpp
// Make oscillation faster overall:
servoSpeed = map(distance, 15, 150, 30, 300);
//                                  ^^  ^^^
//                            Faster range!

// Make oscillation slower:
servoSpeed = map(distance, 15, 150, 100, 800);
```

### Adjust Pitch Range:

```cpp
// Higher pitched sounds:
int pitch = map(distance, 15, 150, 3000, 1000);
//                                ^^^^  ^^^^
//                              Higher range!

// Lower pitched sounds:
int pitch = map(distance, 15, 150, 1000, 200);
```

### Change Detection Range:

```cpp
// Respond from farther away (300cm):
if (distance < 300) {
  // Active zone
}

// Only respond when very close (50cm):
if (distance < 50) {
  // Active zone
}
```

---

## 🎓 Advanced Extensions

### 1. **Add RGB LED**
```cpp
Green:  Far (safe)
Yellow: Medium (caution)
Red:    Close (danger)
```

### 2. **Multiple Servos**
```cpp
// Create array of servos
Servo fans[3];
// Control speed based on direction
```

### 3. **Continuous Rotation Modification**
```cpp
// Modify SG90 for true continuous rotation
// (Requires physical modification - remove potentiometer stop)
```

### 4. **Speed Display as Bar Graph**
```cpp
// LCD custom characters showing speed visually
// ▂▃▅▆█ (increasing speed bars)
```

### 5. **Data Logging**
```cpp
// Log distance vs speed over time
// Analyze response patterns
```

---

## 📊 Servo Technical Details

### SG90 Specifications:

```
Dimension:      23mm × 12mm × 29mm
Weight:         9 grams
Operating Voltage: 4.8V - 6V
Torque:         1.8 kg·cm @ 4.8V
              2.5 kg·cm @ 6V
Speed:          0.1 sec/60° @ 4.8V
              0.08 sec/60° @ 6V
Rotation:       0° - 180° (±10°)
Pulse Width:    500μs - 2500μs
Frequency:      50Hz (20ms period)
Dead Band:      5μs (very precise!)
Temperature:    0°C - 55°C operating
```

### Servo Horn Types:

Your kit includes different attachments:

```
Single Arm:     ─────  (for pointers)
Cross:          ──┼──  (for mechanisms)
Circular:       ◯     (for fan blades) ← Use this!
```

---

## 🎯 Testing Tips

### 1. **Initial Servo Test:**

```cpp
// Upload simple test first:
servo.write(0);    // Should rotate fully left
delay(1000);
servo.write(90);   // Should center
delay(1000);
servo.write(180);  // Should rotate fully right
```

### 2. **Check Servo Direction:**

Some servos are mounted differently. If oscillation seems backwards:
```cpp
// Reverse direction:
position = (position == 0) ? 180 : 0;
// Change to:
position = (position == 180) ? 0 : 180;
```

### 3. **Attach Fan Blade:**

- Cut small propeller from cardboard/paper
- Attach to circular servo horn
- Watch it spin faster as you approach!

### 4. **Monitor Serial Output:**

```
Distance: 100cm | Pitch: 800Hz | Speed: 300ms
Distance: 80cm  | Pitch: 1000Hz | Speed: 220ms
Distance: 50cm  | Pitch: 1400Hz | Speed: 140ms
```

---

## 🔧 Troubleshooting

**Problem: Servo doesn't move**
```
Fix:
  • Check 5V and GND connections
  • Verify signal wire to Pin 11
  • Check servo isn't mechanically jammed
  • Try writing servo.write(90) in setup()
```

**Problem: Servo jitters/vibrates**
```
Fix:
  • Power supply issue - try external 5V supply
  • Loose connections - check all wires firm
  • Signal interference - keep servo wires away from motor wires
```

**Problem: Servo moves but very slow**
```
Fix:
  • Check voltage (should be 4.8-6V, not 3.3V)
  • Servo might be damaged
  • Check if something is blocking movement
```

**Problem: Buzzer silent**
```
Fix:
  • Check buzzer (+) to Pin 8, (-) to GND
  • Verify passive buzzer (not active)
  • Check distance is in active range (<150cm)
```

**Problem: LCD shows wrong info**
```
Fix:
  • Check I2C address (should be 0x27)
  • Verify SDA/SCL connections (A4/A5)
  • Check all grounds connected together
```

---

## 💡 Safety Notes

1. **Servo Power:**
   - Don't stall servo for long periods (overheats)
   - Don't force servo manually when powered
   - Disconnect servo when uploading code

2. **Mechanical:**
   - Ensure fan blade is balanced
   - Don't attach heavy objects to servo
   - Keep fingers away from spinning parts

3. **Electrical:**
   - All GND must be connected together
   - Don't reverse servo polarity (will damage it)
   - Use proper wire gauge for current

---

**Ready to build a proximity-activated fan? Let's wire it up!** 🌀📏

