/*
 * ═══════════════════════════════════════════════════════════════
 *  EXPERIMENT 04: Proximity Alarm - Distance Sensor + Buzzer + LCD
 * ═══════════════════════════════════════════════════════════════
 *
 *  WHAT THIS DOES:
 *  - Measures distance using ultrasonic sensor (HC-SR04)
 *  - Beeps faster as objects get closer
 *  - Shows distance and status on LCD display
 *  - Works like a car parking sensor
 *  - Continuous alarm when very close
 *
 *  HARDWARE NEEDED:
 *  - Arduino Uno R4
 *  - HC-SR04 Ultrasonic Distance Sensor
 *  - Buzzer (Active or Passive)
 *  - 16x2 LCD with I2C (already connected!)
 *  - 5 jumper wires
 *
 *  CONNECTIONS:
 *  HC-SR04:
 *    VCC  → Arduino 5V
 *    TRIG → Arduino Pin 9
 *    ECHO → Arduino Pin 10
 *    GND  → Arduino GND
 *
 *  Buzzer:
 *    (+)  → Arduino Pin 8
 *    (-)  → Arduino GND
 *
 *  LCD (I2C) - Already connected:
 *    GND  → Arduino GND
 *    VCC  → Arduino 5V
 *    SDA  → Arduino A4
 *    SCL  → Arduino A5
 *
 * ═══════════════════════════════════════════════════════════════
 */

// ─────────────────────────────────────────────────────────────────
// LIBRARY INCLUDES
// ─────────────────────────────────────────────────────────────────

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ─────────────────────────────────────────────────────────────────
// HARDWARE CONFIGURATION
// ─────────────────────────────────────────────────────────────────

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD at address 0x27

// ─────────────────────────────────────────────────────────────────
// PIN CONFIGURATION
// ─────────────────────────────────────────────────────────────────

const int trigPin = 9;      // Trigger pin for ultrasonic sensor
const int echoPin = 10;     // Echo pin for ultrasonic sensor
const int buzzerPin = 8;    // Buzzer pin

// ─────────────────────────────────────────────────────────────────
// GLOBAL VARIABLES
// ─────────────────────────────────────────────────────────────────

long duration;              // Time for sound wave to return (microseconds)
int distance;               // Calculated distance in centimeters
unsigned long lastBeep = 0; // Last time buzzer beeped
int beepInterval = 1000;    // Time between beeps (milliseconds)

// ─────────────────────────────────────────────────────────────────
// DISTANCE THRESHOLDS (You can adjust these!)
// ─────────────────────────────────────────────────────────────────

const int ALARM_DISTANCE = 15;      // Continuous alarm below this (cm)
const int CLOSE_DISTANCE = 30;      // Very fast beeps
const int MEDIUM_DISTANCE = 60;     // Medium speed beeps
const int FAR_DISTANCE = 100;       // Slow beeps
const int MAX_DISTANCE = 200;       // Silent above this

// ─────────────────────────────────────────────────────────────────
// SETUP FUNCTION - Runs once on startup
// ─────────────────────────────────────────────────────────────────

void setup() {

  // ┌───────────────────────────────────────────────────────────┐
  // │ 1. Initialize Serial Monitor for debugging               │
  // └───────────────────────────────────────────────────────────┘

  Serial.begin(9600);
  Serial.println("═══════════════════════════════════════");
  Serial.println("   PROXIMITY ALARM - Distance Sensor");
  Serial.println("═══════════════════════════════════════");
  Serial.println();
  Serial.println("Initializing...");

  // ┌───────────────────────────────────────────────────────────┐
  // │ 2. Configure Ultrasonic Sensor Pins                      │
  // └───────────────────────────────────────────────────────────┘

  pinMode(trigPin, OUTPUT);   // Trigger sends ultrasonic pulse
  pinMode(echoPin, INPUT);    // Echo receives reflected pulse

  // ┌───────────────────────────────────────────────────────────┐
  // │ 3. Configure Buzzer Pin                                   │
  // └───────────────────────────────────────────────────────────┘

  pinMode(buzzerPin, OUTPUT);

  // ┌───────────────────────────────────────────────────────────┐
  // │ 4. Initialize LCD Display                                 │
  // └───────────────────────────────────────────────────────────┘

  lcd.init();         // Initialize the LCD
  lcd.backlight();    // Turn on backlight

  // Show welcome screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parking Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  Serial.println("✓ Sensor initialized");
  Serial.println("✓ Buzzer ready");
  Serial.println("✓ LCD ready");
  Serial.println();
  Serial.println("Point sensor at object and observe!");
  Serial.println("Closer object = Faster beeps");
  Serial.println();
  Serial.println("Distance (cm) | Status");
  Serial.println("─────────────────────────");

  delay(2000);
  lcd.clear();
}

// ─────────────────────────────────────────────────────────────────
// MAIN LOOP - Runs continuously
// ─────────────────────────────────────────────────────────────────

void loop() {

  // ┌───────────────────────────────────────────────────────────┐
  // │ 1. Measure Distance                                       │
  // └───────────────────────────────────────────────────────────┘

  distance = measureDistance();

  // ┌───────────────────────────────────────────────────────────┐
  // │ 2. Control Buzzer Based on Distance                      │
  // └───────────────────────────────────────────────────────────┘

  controlBuzzer(distance);

  // ┌───────────────────────────────────────────────────────────┐
  // │ 3. Display Distance on Serial Monitor                    │
  // └───────────────────────────────────────────────────────────┘

  displayStatus(distance);

  // Small delay to avoid overwhelming Serial Monitor
  delay(100);
}

// ═══════════════════════════════════════════════════════════════
// FUNCTION: Measure Distance Using Ultrasonic Sensor
// ═══════════════════════════════════════════════════════════════

int measureDistance() {

  /*
   * HOW IT WORKS:
   * 1. Send 10μs pulse to TRIG pin
   * 2. Sensor emits 8 ultrasonic sound pulses (40kHz)
   * 3. Sound travels to object and bounces back
   * 4. ECHO pin goes HIGH when sound is detected
   * 5. Measure how long ECHO stays HIGH
   * 6. Calculate distance from time
   *
   * FORMULA:
   * Distance (cm) = (Time in microseconds × Speed of Sound) / 2
   * Speed of Sound = 0.034 cm/μs (at room temperature)
   * Divide by 2 because sound travels TO and FROM object
   */

  // ┌───────────────────────────────────────────────────────────┐
  // │ 1. Ensure trigger pin is LOW (clean state)               │
  // └───────────────────────────────────────────────────────────┘

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // Wait 2μs for stable LOW

  // ┌───────────────────────────────────────────────────────────┐
  // │ 2. Send 10μs HIGH pulse to trigger sensor                │
  // └───────────────────────────────────────────────────────────┘

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);  // 10μs pulse
  digitalWrite(trigPin, LOW);

  /*
   * This 10μs pulse tells the sensor:
   * "Start sending ultrasonic pulses now!"
   */

  // ┌───────────────────────────────────────────────────────────┐
  // │ 3. Measure how long ECHO pin stays HIGH                  │
  // └───────────────────────────────────────────────────────────┘

  duration = pulseIn(echoPin, HIGH);

  /*
   * pulseIn() function:
   * - Waits for pin to go HIGH
   * - Starts timer
   * - Waits for pin to go LOW
   * - Returns duration in microseconds
   *
   * This duration = time for sound to travel TO object and BACK
   */

  // ┌───────────────────────────────────────────────────────────┐
  // │ 4. Calculate distance from duration                      │
  // └───────────────────────────────────────────────────────────┘

  int calculatedDistance = (duration * 0.034) / 2;

  /*
   * MATH BREAKDOWN:
   *
   * duration = microseconds (μs)
   * speed of sound = 343 m/s = 0.034 cm/μs
   *
   * distance = duration × 0.034 cm/μs
   *
   * Divide by 2 because:
   * - Sound goes TO object (distance D)
   * - Sound comes BACK (distance D)
   * - Total travel = 2D
   * - So actual distance = Total / 2
   */

  // ┌───────────────────────────────────────────────────────────┐
  // │ 5. Return the calculated distance                        │
  // └───────────────────────────────────────────────────────────┘

  return calculatedDistance;
}

// ═══════════════════════════════════════════════════════════════
// FUNCTION: Control Buzzer Based on Distance
// ═══════════════════════════════════════════════════════════════

void controlBuzzer(int dist) {

  /*
   * BEHAVIOR:
   * - Very close (<15cm):  Continuous ALARM
   * - Close (15-30cm):     Very fast beeps (10/sec)
   * - Medium (30-60cm):    Fast beeps (4/sec)
   * - Far (60-100cm):      Medium beeps (2/sec)
   * - Very far (100-200cm): Slow beeps (1/sec)
   * - Too far (>200cm):    Silent
   */

  // ┌───────────────────────────────────────────────────────────┐
  // │ ZONE 1: ALARM - Continuous tone when VERY close          │
  // └───────────────────────────────────────────────────────────┘

  if (dist < ALARM_DISTANCE) {

    // Continuous high-pitched alarm
    tone(buzzerPin, 2000);  // 2000 Hz = high pitch

    /*
     * tone(pin, frequency):
     * Plays continuous tone until noTone() is called
     */
  }

  // ┌───────────────────────────────────────────────────────────┐
  // │ ZONE 2-5: Beeping with intervals based on distance       │
  // └───────────────────────────────────────────────────────────┘

  else if (dist < MAX_DISTANCE) {

    // Stop continuous tone if it was playing
    noTone(buzzerPin);

    // ┌─────────────────────────────────────────────────────────┐
    // │ Calculate beep interval using map()                    │
    // └─────────────────────────────────────────────────────────┘

    /*
     * map() function maps one range to another:
     *
     * map(value, fromLow, fromHigh, toLow, toHigh)
     *
     * Example:
     * dist = 15cm  →  beepInterval = 50ms (very fast)
     * dist = 200cm →  beepInterval = 1000ms (slow)
     * dist = 100cm →  beepInterval = ~500ms (medium)
     *
     * Closer distance → Smaller interval → Faster beeps!
     */

    beepInterval = map(dist, ALARM_DISTANCE, MAX_DISTANCE, 50, 1000);

    /*
     * WHAT THIS DOES:
     * Maps distance (15-200cm) to interval (50-1000ms)
     *
     * Linear interpolation:
     * Close (15cm):  50ms intervals  = 20 beeps/sec
     * Medium (100cm): 500ms intervals = 2 beeps/sec
     * Far (200cm):   1000ms intervals = 1 beep/sec
     */

    // ┌─────────────────────────────────────────────────────────┐
    // │ Beep if enough time has passed (non-blocking)          │
    // └─────────────────────────────────────────────────────────┘

    if (millis() - lastBeep > beepInterval) {

      // Play short beep
      tone(buzzerPin, 1000, 100);  // 1000 Hz for 100ms

      /*
       * tone(pin, frequency, duration):
       * - Plays 1000 Hz tone
       * - For 100 milliseconds
       * - Then automatically stops
       */

      // Update last beep time
      lastBeep = millis();
    }
  }

  // ┌───────────────────────────────────────────────────────────┐
  // │ ZONE 6: Silent - Object too far away                     │
  // └───────────────────────────────────────────────────────────┘

  else {
    // Object beyond range - silence
    noTone(buzzerPin);
  }
}

// ═══════════════════════════════════════════════════════════════
// FUNCTION: Display Status on Serial Monitor
// ═══════════════════════════════════════════════════════════════

void displayStatus(int dist) {

  /*
   * Shows distance and status message on:
   * - Serial Monitor (for debugging)
   * - LCD Display (for user to see)
   */

  // ┌───────────────────────────────────────────────────────────┐
  // │ 1. Print to Serial Monitor                                │
  // └───────────────────────────────────────────────────────────┘

  Serial.print(dist);
  Serial.print(" cm        | ");

  // ┌───────────────────────────────────────────────────────────┐
  // │ 2. Update LCD and Serial based on distance zones         │
  // └───────────────────────────────────────────────────────────┘

  // Row 0: Distance
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(dist);
  lcd.print(" cm    ");  // Extra spaces to clear old digits

  // Row 1: Status message
  lcd.setCursor(0, 1);

  if (dist < ALARM_DISTANCE) {
    // ALARM Zone
    lcd.print("ALARM!!! CLOSE! ");  // 16 characters exactly
    Serial.println("🚨 ALARM!!! Too close!");
  }
  else if (dist < CLOSE_DISTANCE) {
    // DANGER Zone
    lcd.print("DANGER-VeryClose");   // 16 characters exactly
    Serial.println("⚠️  DANGER - Very close!");
  }
  else if (dist < MEDIUM_DISTANCE) {
    // WARNING Zone
    lcd.print("WARNING-Close   ");   // 16 characters
    Serial.println("⚡ WARNING - Getting close");
  }
  else if (dist < FAR_DISTANCE) {
    // CAUTION Zone
    lcd.print("CAUTION-Medium  ");   // 16 characters
    Serial.println("⚠️  CAUTION - Medium distance");
  }
  else if (dist < MAX_DISTANCE) {
    // NOTICE Zone
    lcd.print("NOTICE-Detected ");   // 16 characters
    Serial.println("ℹ️  NOTICE - Far but detected");
  }
  else {
    // SAFE Zone
    lcd.print("SAFE-Nothing    ");   // 16 characters
    Serial.println("✓  SAFE - Nothing detected");
  }
}

// ═══════════════════════════════════════════════════════════════
// 📚 LEARNING NOTES
// ═══════════════════════════════════════════════════════════════

/*
 * KEY CONCEPTS DEMONSTRATED:
 *
 * 1. ULTRASONIC DISTANCE MEASUREMENT
 *    - Send trigger pulse (10μs)
 *    - Measure echo pulse duration
 *    - Calculate distance from time
 *    - Uses speed of sound (343 m/s)
 *
 * 2. PULSE WIDTH MEASUREMENT
 *    - pulseIn(pin, state) measures HIGH or LOW duration
 *    - Returns time in microseconds
 *    - Essential for distance sensors
 *
 * 3. MAP FUNCTION
 *    - map(value, fromLow, fromHigh, toLow, toHigh)
 *    - Converts one range to another
 *    - Linear interpolation
 *    - Perfect for scaling values
 *
 * 4. TONE GENERATION
 *    - tone(pin, frequency) = continuous sound
 *    - tone(pin, freq, duration) = timed sound
 *    - noTone(pin) = stop sound
 *    - Frequency in Hz (1000 = 1kHz)
 *
 * 5. NON-BLOCKING TIMING
 *    - Using millis() for timed events
 *    - Allows continuous distance measurement
 *    - While controlling beep timing
 *
 * 6. STATE-BASED CONTROL
 *    - Different behaviors for different distances
 *    - if-else chain creates zones
 *    - Each zone has unique behavior
 *
 * REAL-WORLD PHYSICS:
 *
 * Speed of Sound:
 * - Varies with temperature: ~343 m/s at 20°C
 * - Faster in warm air, slower in cold
 * - Formula: v = 331.3 + (0.606 × temperature°C)
 *
 * Ultrasonic Frequency:
 * - HC-SR04 uses 40kHz (40,000 Hz)
 * - Human hearing: 20 Hz - 20 kHz
 * - 40kHz is ABOVE human hearing (ultrasonic)
 *
 * Detection Range:
 * - Minimum: ~2cm (too close for echo separation)
 * - Maximum: ~400cm (signal too weak)
 * - Optimal: 5cm - 300cm
 *
 * APPLICATIONS:
 * - Car parking sensors
 * - Robot obstacle avoidance
 * - Automatic door openers
 * - Level measurement (liquids in tanks)
 * - Distance measuring tools
 * - Security perimeter alarms
 *
 * EXTENSIONS YOU CAN TRY:
 * 1. Add LED indicators (Green=Safe, Red=Close)
 * 2. Display distance on LCD
 * 3. Log data over time (graph)
 * 4. Add multiple sensors (360° coverage)
 * 5. Change buzzer pitch based on distance
 * 6. Add button to adjust sensitivity
 * 7. Create "safe zone" perimeter alarm
 */

// ═══════════════════════════════════════════════════════════════
// END OF CODE
// ═══════════════════════════════════════════════════════════════
