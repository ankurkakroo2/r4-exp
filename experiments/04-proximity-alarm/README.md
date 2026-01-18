# Experiment 04: Proximity Alarm - Distance Sensor + Buzzer

## What We're Building

A **proximity alarm system** that:
1. 📏 Measures distance using ultrasonic sensor
2. 🔊 Beeps faster as you get closer
3. 🚗 Works like a car parking sensor
4. 🎯 Visual and audio feedback

**Behavior:**
- **Far away (>100cm):** Slow beeps (1 per second)
- **Medium distance (50-100cm):** Medium beeps (2-3 per second)
- **Close (20-50cm):** Fast beeps (5+ per second)
- **Very close (<20cm):** Continuous alarm!

---

## 🎓 New Concepts You'll Learn

### 1. **Ultrasonic Sensor (HC-SR04) - How It Works**

**Think of it like SONAR (like bats and submarines!):**

```
Sensor → Sends ultrasonic sound wave → [Object] → Echo bounces back → Sensor receives

Time taken = Distance to object!
```

**The Physics:**
```
Speed of sound = 343 meters/second (at room temperature)

Distance = (Time × Speed of Sound) / 2

Why divide by 2?
Sound travels TO object and BACK, so total distance is 2× the actual distance!
```

**How HC-SR04 Works:**

```
HC-SR04 Sensor
┌──────────────┐
│  [T] [R]     │  T = Transmitter (sends ultrasound)
│   █   █      │  R = Receiver (detects echo)
└──────────────┘

Process:
1. Arduino sends 10μs pulse to TRIG pin
2. Sensor emits 8 ultrasonic pulses (40kHz - inaudible to humans)
3. Sound waves travel through air
4. Hit object and bounce back
5. Receiver detects echo
6. ECHO pin goes HIGH for duration = travel time
7. Arduino measures this time
8. Calculate: distance = (time × 0.034) / 2
```

**Specifications:**
- **Range:** 2cm to 400cm (about 4 meters)
- **Accuracy:** ±3mm
- **Angle:** 15° cone (best for objects directly in front)
- **Frequency:** 40kHz ultrasonic (humans can't hear it)

---

### 2. **Distance to Frequency Mapping**

**The Algorithm:**

```cpp
Closer distance → Faster beeps

Example mapping:
Distance (cm)  |  Beep Interval (ms)  |  Beeps per second
─────────────────────────────────────────────────────────
> 200          |  No beeps            |  0 (silent)
100-200        |  1000ms              |  1 beep/sec
50-100         |  500ms               |  2 beeps/sec
30-50          |  250ms               |  4 beeps/sec
15-30          |  100ms               |  10 beeps/sec
< 15           |  Continuous tone     |  ALARM!
```

**Using map() function:**
```cpp
// Maps one range to another
beepDelay = map(distance, 5, 100, 50, 1000);
//              ^^^^^^^^   ^^^^^  ^^  ^^^^
//              input      input  output
//              value      range  range

// When distance = 5cm  → beepDelay = 50ms (very fast)
// When distance = 100cm → beepDelay = 1000ms (slow)
// Automatically scales in between!
```

---

### 3. **Pulse Width Measurement**

**pulseIn() function:**
```cpp
duration = pulseIn(echoPin, HIGH);
```

**What it does:**
- Waits for pin to go HIGH
- Starts timer
- Waits for pin to go LOW
- Stops timer
- Returns duration in microseconds

**This measures how long the ECHO pin stays HIGH = sound travel time!**

---

### 4. **Non-Blocking Beeping**

**Challenge:** Make beeps while continuously measuring distance

**Bad approach (blocking):**
```cpp
void loop() {
  distance = measureDistance();
  tone(buzzer, 1000, 500);  // Plays for 500ms
  delay(500);               // BLOCKS! Can't measure during this time
}
```

**Good approach (non-blocking):**
```cpp
unsigned long lastBeep = 0;
int beepInterval = 1000;  // Updated based on distance

void loop() {
  distance = measureDistance();  // Always measuring

  beepInterval = map(distance, 5, 100, 50, 1000);  // Update interval

  if (millis() - lastBeep > beepInterval) {
    beep();  // Quick beep
    lastBeep = millis();
  }
}
```

---

## 🔌 Components Needed

### From Your Kit:
1. ✅ Arduino Uno R4
2. ✅ HC-SR04 Ultrasonic Sensor
3. ✅ Buzzer (Active OR Passive - both work!)
4. ✅ 4 jumper wires (Male-to-Female or Male-to-Male)

---

## 🔧 Wiring

### HC-SR04 Ultrasonic Sensor Pins:

```
HC-SR04 (looking at front):
┌────────────────┐
│   [T]    [R]   │  ← Transmitter & Receiver (sensors)
│                │
│ VCC TRIG ECHO GND
│  •   •    •   •│  ← 4 pins
└────────────────┘
```

### Connection Table:

```
HC-SR04 Pin      →    Arduino Pin
────────────────────────────────────
VCC              →    5V
TRIG (Trigger)   →    Pin 9
ECHO             →    Pin 10
GND              →    GND

Buzzer           →    Arduino Pin
────────────────────────────────────
(+) Positive     →    Pin 8
(-) Negative     →    GND
```

### Visual Wiring:

```
HC-SR04 Sensor              Arduino Uno R4
┌──────────────┐           ┌──────────────┐
│   [T]  [R]   │           │              │
│ VCC TRIG ECHO GND        │   POWER      │
│  •   •   •   •│          │   ─────      │
└──┼───┼───┼───┼┘          │   GND  ●─────┼──(Sensor GND, Buzzer -)
   │   │   │   │           │   GND        │
   │   │   │   └───────────┤   5V   ●─────┼──(Sensor VCC)
   │   │   │               │              │
   │   │   │               │  DIGITAL     │
   │   │   │               │  ───────     │
   │   │   │               │   Pin 8 ●────┼──(Buzzer +)
   │   │   │               │   Pin 9 ●────┼──(Sensor TRIG)
   │   │   └───────────────┤   Pin 10●────┼──(Sensor ECHO)
   │   └───────────────────┤              │
   └───────────────────────┘              │
                                          └──────────────┘

Buzzer
┌──────┐
│ (+)  │───────→ Pin 8
│ (-)  │───────→ GND
└──────┘
```

---

## 💡 How the Code Works

### Main Algorithm:

```
Loop continuously:
  ├─ 1. Trigger ultrasonic pulse (10μs)
  ├─ 2. Measure echo return time
  ├─ 3. Calculate distance from time
  ├─ 4. Map distance → beep frequency
  ├─ 5. Beep if interval elapsed
  └─ 6. Repeat
```

### Distance Calculation:

```cpp
// Send 10μs pulse
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Measure echo pulse duration
duration = pulseIn(echoPin, HIGH);

// Calculate distance
// Speed of sound = 0.034 cm/μs
// Divide by 2 because sound goes there and back
distance = (duration * 0.034) / 2;
```

### Beep Frequency Logic:

```cpp
if (distance < 15) {
  // VERY CLOSE: Continuous alarm
  tone(buzzer, 2000);  // High-pitched continuous tone
}
else if (distance < 200) {
  // Map distance to beep interval
  int beepInterval = map(distance, 15, 200, 50, 1000);

  // Beep periodically
  if (millis() - lastBeep > beepInterval) {
    tone(buzzer, 1000, 100);  // Short beep
    lastBeep = millis();
  }
}
else {
  // TOO FAR: Silent
  noTone(buzzer);
}
```

---

## 🎯 Expected Behavior

### Test Sequence:

**1. Nothing nearby (>200cm):**
```
Serial Monitor: "Distance: 250cm - Safe"
Buzzer: Silent
```

**2. Medium distance (100cm):**
```
Serial Monitor: "Distance: 100cm - Medium"
Buzzer: BEEP... BEEP... BEEP...
        (1 second intervals)
```

**3. Getting closer (50cm):**
```
Serial Monitor: "Distance: 50cm - Close"
Buzzer: BEEP..BEEP..BEEP..BEEP
        (Half second intervals)
```

**4. Very close (20cm):**
```
Serial Monitor: "Distance: 20cm - WARNING!"
Buzzer: BEEPBEEPBEEPBEEPBEEP
        (Very fast - 10 per second)
```

**5. Touching sensor (<15cm):**
```
Serial Monitor: "Distance: 10cm - ALARM!!!"
Buzzer: BEEEEEEEEEEEEEEE
        (Continuous tone)
```

---

## 🎨 Code Features

### 1. **Real-time Distance Display**
```cpp
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
```

### 2. **Smart Beep Intervals**
```cpp
// Automatically adjusts beep speed based on distance
beepInterval = map(distance, 15, 200, 50, 1000);
```

### 3. **Different Alert Zones**
```cpp
if (distance < 15)      ALARM!
if (distance < 30)      DANGER
if (distance < 50)      WARNING
if (distance < 100)     CAUTION
if (distance < 200)     NOTICE
else                    SAFE
```

### 4. **Smooth Transitions**
No sudden jumps - beep frequency gradually increases

---

## 🚀 Real-World Applications

This exact principle is used in:

1. **Car Parking Sensors**
   - Beeps faster as you approach wall
   - Some cars show distance on display

2. **Automatic Doors**
   - Detect people approaching
   - Open when within range

3. **Robot Obstacle Avoidance**
   - Detect objects ahead
   - Stop or turn away

4. **Security Systems**
   - Perimeter detection
   - Intrusion alarms

5. **Level Measurement**
   - Liquid level in tanks
   - Distance to objects

6. **Touchless Interfaces**
   - Hand gesture detection
   - Proximity-based controls

---

## 🔧 Calibration & Tuning

### Adjust Sensitivity:

**Make it more sensitive (beep sooner):**
```cpp
// Change maximum distance from 200 to 300
if (distance < 300) {  // Increased range
  beepInterval = map(distance, 15, 300, 50, 1000);
}
```

**Make beeps faster:**
```cpp
// Change minimum interval from 50ms to 20ms
beepInterval = map(distance, 15, 200, 20, 1000);
//                                     ^^
//                               Faster minimum!
```

**Change alarm threshold:**
```cpp
// Alarm at 20cm instead of 15cm
if (distance < 20) {  // Changed from 15
  tone(buzzer, 2000);
}
```

---

## 🎓 Advanced Extensions

### 1. **Add LCD Display**
```cpp
lcd.setCursor(0, 0);
lcd.print("Distance: ");
lcd.print(distance);
lcd.print("cm  ");

lcd.setCursor(0, 1);
if (distance < 15) lcd.print("ALARM!!!    ");
else if (distance < 50) lcd.print("WARNING!    ");
else lcd.print("Safe        ");
```

### 2. **Add LEDs (Visual Feedback)**
```cpp
Green LED:  Far (safe)
Yellow LED: Medium (caution)
Red LED:    Close (danger)
```

### 3. **Varying Pitch**
```cpp
// Higher pitch = closer object
int pitch = map(distance, 15, 200, 2000, 200);
tone(buzzer, pitch, 100);
```

### 4. **Data Logging**
```cpp
// Record distances over time
// Plot on Serial Plotter
// Detect patterns
```

### 5. **Multiple Sensors**
```cpp
// Front, back, left, right
// 360° obstacle detection
// Robot navigation
```

---

## 📊 Understanding the Physics

### Why Ultrasonic?

**Ultrasonic = Above human hearing range**

```
Human hearing: 20 Hz - 20,000 Hz (20 kHz)
HC-SR04 uses: 40,000 Hz (40 kHz)

Benefits:
✓ Inaudible to humans (not annoying)
✓ Good directionality (focused beam)
✓ Not affected by light/color
✓ Works in dark
✓ Low power consumption
```

### Limitations:

1. **Soft Materials:**
   - Absorb sound (fabric, foam)
   - May not reflect well

2. **Angled Surfaces:**
   - Reflect sound away
   - Sensor might not detect

3. **Temperature:**
   - Sound speed varies with temperature
   - ±2°C can affect accuracy slightly

4. **Narrow Objects:**
   - Smaller than sensor beam might be missed
   - Best for larger surfaces

5. **Maximum Range:**
   - 400cm theoretical
   - Practical reliable range: ~200-300cm

---

## 🎯 Testing Tips

### 1. **Test Objects:**

**Good test objects:**
- ✓ Your hand (flat surface)
- ✓ Books
- ✓ Cardboard box
- ✓ Wall

**Challenging objects:**
- ✗ Wire mesh (sound passes through)
- ✗ Fabric (absorbs sound)
- ✗ Sloped surfaces (reflects away)

### 2. **Optimal Positioning:**

```
Sensor → [Hand]
   ↑
   └─ Keep perpendicular for best results

Good:  Sensor facing object directly
       ════════════→ [Object]

Bad:   Sensor at angle
       ═══════╱
             ╱
        [Object] ← Sound reflects away
```

### 3. **Serial Monitor:**

```cpp
// Watch distance values
// Smooth? → Good
// Jumpy? → Check wiring, positioning
```

---

## 🎵 Sound Patterns You Can Try

### 1. **Parking Sensor Style:**
```cpp
Beep... Beep... Beep.. Beep. BeepBeepBeepBEEEEEE
```

### 2. **Geiger Counter Style:**
```cpp
Click. Click.. Click... ClickClickClickClick
```

### 3. **Missile Lock Style:**
```cpp
Slow beep → Fast beep → Continuous high pitch
```

### 4. **Musical Scale:**
```cpp
// Play different notes based on distance
// Far = low C, Close = high C
```

---

## 🔬 Scientific Experiments

### 1. **Speed of Sound Measurement**
```cpp
// Measure known distance
// Calculate actual speed of sound
// Compare to 343 m/s
```

### 2. **Material Reflectivity**
```cpp
// Test different materials
// Which reflects best?
// Carpet vs wood vs metal
```

### 3. **Angle Testing**
```cpp
// How far off-axis can object be detected?
// Map the detection cone
```

---

**Ready to build your proximity alarm? Let's wire it up!** 🚗📏
