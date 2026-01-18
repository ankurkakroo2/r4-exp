/*
 * ═══════════════════════════════════════════════════════════════
 *  EXPERIMENT 05: Proximity Fan - Servo Speed + Mario Music
 * ═══════════════════════════════════════════════════════════════
 *
 *  WHAT THIS DOES:
 *  - Measures distance with HC-SR04 ultrasonic sensor
 *  - Oscillates servo faster as objects get closer
 *  - Plays Mario theme FASTER as objects approach
 *  - Shows real-time distance, tempo, and speed on LCD
 *
 *  BEHAVIOR:
 *  Far (>150cm):     Silent, servo stopped
 *  Medium (100cm):   Slow Mario theme, slow oscillation
 *  Close (50cm):     Medium tempo, medium speed
 *  Very close (<20cm): Fast Mario theme, fast "fan" motion
 *
 *  HARDWARE NEEDED:
 *  - Arduino Uno R4
 *  - HC-SR04 Ultrasonic Sensor
 *  - SG90 Servo Motor
 *  - Passive Buzzer
 *  - 16x2 LCD with I2C
 *
 *  CONNECTIONS:
 *  Servo Brown/Black  → GND (via breadboard)
 *  Servo Red          → 5V (via breadboard)
 *  Servo Orange       → Arduino Pin 11 (PWM)
 *  Buzzer (+)         → Arduino Pin 8
 *  Buzzer (-)         → GND (via breadboard)
 *
 * ═══════════════════════════════════════════════════════════════
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ─────────────────────────────────────────────────────────────────
// HARDWARE CONFIGURATION
// ─────────────────────────────────────────────────────────────────

// Ultrasonic Sensor
const int trigPin = 9;      // HC-SR04 TRIG pin
const int echoPin = 10;     // HC-SR04 ECHO pin

// Servo Motor
const int servoPin = 11;    // Servo control pin (must be PWM capable)
Servo fanServo;             // Servo object

// Buzzer
const int buzzerPin = 8;    // Passive buzzer

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD at I2C address 0x27

// ─────────────────────────────────────────────────────────────────
// MUSICAL NOTES (Frequencies in Hz)
// ─────────────────────────────────────────────────────────────────

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

#define REST     0     // Silence (no tone)

// ─────────────────────────────────────────────────────────────────
// SUPER MARIO THEME MELODY
// ─────────────────────────────────────────────────────────────────

int marioMelody[] = {
  NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5, REST,
  NOTE_G5, REST, REST, REST, NOTE_G4, REST, REST, REST,

  NOTE_C5, REST, REST, NOTE_G4, REST, REST, NOTE_E4, REST,
  REST, NOTE_A4, REST, NOTE_B4, REST, NOTE_AS4, NOTE_A4, REST,

  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, REST, NOTE_F5, NOTE_G5, REST,
  NOTE_E5, REST, NOTE_C5, NOTE_D5, NOTE_B4, REST, REST, REST
};

int marioDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,

  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8,

  6, 6, 6, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8
};

int melodyLength = sizeof(marioMelody) / sizeof(marioMelody[0]);

// ─────────────────────────────────────────────────────────────────
// DISTANCE THRESHOLDS (in centimeters)
// ─────────────────────────────────────────────────────────────────

const int ALARM_DISTANCE = 15;      // <15cm: maximum speed & fastest tempo
const int VERY_CLOSE = 30;          // 15-30cm: very fast
const int CLOSE_DISTANCE = 60;      // 30-60cm: fast
const int MEDIUM_DISTANCE = 100;    // 60-100cm: medium
const int FAR_DISTANCE = 150;       // 100-150cm: slow
const int MAX_DISTANCE = 200;       // >150cm: stopped

// ─────────────────────────────────────────────────────────────────
// TIMING VARIABLES
// ─────────────────────────────────────────────────────────────────

unsigned long lastServoMove = 0;    // Last time servo moved
unsigned long lastLCDUpdate = 0;    // Last time LCD updated
unsigned long lastDistanceMeasure = 0; // Last distance measurement
unsigned long lastNoteTime = 0;     // Last time a note was played

int servoSpeed = 500;               // Delay between servo moves (ms)
int currentServoPosition = 90;      // Current servo angle (0 or 180)
bool servoAtLeft = true;            // Track servo position

int currentNote = 0;                // Current position in melody
int tempoMultiplier = 100;          // Tempo as percentage (100 = normal)

const int LCD_UPDATE_INTERVAL = 200;      // Update LCD every 200ms
const int DISTANCE_MEASURE_INTERVAL = 50; // Measure distance every 50ms

// ─────────────────────────────────────────────────────────────────
// SENSOR DATA
// ─────────────────────────────────────────────────────────────────

long duration;                      // Pulse duration from sensor
int distance = 0;                   // Calculated distance in cm

// ─────────────────────────────────────────────────────────────────
// SETUP FUNCTION
// ─────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(9600);
  Serial.println("🎵🌀 Proximity Fan + Mario Music - Starting!");

  // ┌───────────────────────────────────────────────────────────┐
  // │ Initialize Ultrasonic Sensor                             │
  // └───────────────────────────────────────────────────────────┘

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // ┌───────────────────────────────────────────────────────────┐
  // │ Initialize Servo Motor                                   │
  // └───────────────────────────────────────────────────────────┘

  fanServo.attach(servoPin);

  // Center servo at startup
  fanServo.write(90);
  currentServoPosition = 90;

  Serial.println("✓ Servo initialized at 90° (center)");

  // ┌───────────────────────────────────────────────────────────┐
  // │ Initialize Buzzer                                        │
  // └───────────────────────────────────────────────────────────┘

  pinMode(buzzerPin, OUTPUT);
  noTone(buzzerPin);  // Start silent

  // ┌───────────────────────────────────────────────────────────┐
  // │ Initialize LCD Display                                   │
  // └───────────────────────────────────────────────────────────┘

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Show startup message
  lcd.setCursor(0, 0);
  lcd.print("Proximity Fan");
  lcd.setCursor(0, 1);
  lcd.print("+ Mario Music!");

  delay(2000);
  lcd.clear();

  Serial.println("🚀 System ready! Move hand near sensor...");
  Serial.println();
}

// ─────────────────────────────────────────────────────────────────
// MAIN LOOP
// ─────────────────────────────────────────────────────────────────

void loop() {

  // ┌───────────────────────────────────────────────────────────┐
  // │ 1. Measure Distance (every 50ms)                         │
  // └───────────────────────────────────────────────────────────┘

  if (millis() - lastDistanceMeasure > DISTANCE_MEASURE_INTERVAL) {
    distance = measureDistance();
    lastDistanceMeasure = millis();
  }

  // ┌───────────────────────────────────────────────────────────┐
  // │ 2. Calculate Servo Speed and Music Tempo                │
  // └───────────────────────────────────────────────────────────┘

  calculateSpeedAndTempo(distance);

  // ┌───────────────────────────────────────────────────────────┐
  // │ 3. Control Servo (non-blocking oscillation)             │
  // └───────────────────────────────────────────────────────────┘

  controlServo();

  // ┌───────────────────────────────────────────────────────────┐
  // │ 4. Play Mario Music (non-blocking, variable tempo)      │
  // └───────────────────────────────────────────────────────────┘

  playMarioMusic();

  // ┌───────────────────────────────────────────────────────────┐
  // │ 5. Update LCD Display (every 200ms)                      │
  // └───────────────────────────────────────────────────────────┘

  if (millis() - lastLCDUpdate > LCD_UPDATE_INTERVAL) {
    updateLCD(distance);
    lastLCDUpdate = millis();
  }
}

// ─────────────────────────────────────────────────────────────────
// MEASURE DISTANCE WITH ULTRASONIC SENSOR
// ─────────────────────────────────────────────────────────────────

int measureDistance() {
  /*
   * Sends ultrasonic pulse and measures echo return time
   * Returns: Distance in centimeters
   */

  // Send 10μs trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance
  // Speed of sound = 343 m/s = 0.034 cm/μs
  // Distance = (Time × Speed) / 2  (divide by 2 for round trip)
  int calculatedDistance = (duration * 0.034) / 2;

  // Limit to reasonable range
  if (calculatedDistance > 400 || calculatedDistance <= 0) {
    calculatedDistance = 400;  // Out of range
  }

  return calculatedDistance;
}

// ─────────────────────────────────────────────────────────────────
// CALCULATE SERVO SPEED AND MUSIC TEMPO FROM DISTANCE
// ─────────────────────────────────────────────────────────────────

void calculateSpeedAndTempo(int dist) {
  /*
   * Maps distance to servo oscillation speed and music tempo
   * Closer = faster servo, faster music
   */

  if (dist < FAR_DISTANCE) {

    // ┌─────────────────────────────────────────────────────────┐
    // │ Map distance to servo speed (delay between moves)      │
    // │ Closer distance (15cm) → faster speed (150ms delay)    │
    // │ Farther distance (150cm) → slower speed (600ms delay)  │
    // │                                                         │
    // │ Note: SG90 needs ~300ms to complete 180° swing         │
    // │ Minimum 150ms allows partial completion (fan effect)   │
    // └─────────────────────────────────────────────────────────┘

    servoSpeed = map(dist, ALARM_DISTANCE, FAR_DISTANCE, 150, 600);

    // Ensure valid range
    servoSpeed = constrain(servoSpeed, 150, 600);

    // ┌─────────────────────────────────────────────────────────┐
    // │ Map distance to music tempo (playback speed)           │
    // │ Closer distance (15cm) → faster tempo (300%)           │
    // │ Farther distance (150cm) → slower tempo (50%)          │
    // │                                                         │
    // │ 100% = normal speed                                    │
    // │ 200% = 2x faster (half the note duration)              │
    // │ 50% = 2x slower (double the note duration)             │
    // └─────────────────────────────────────────────────────────┘

    tempoMultiplier = map(dist, ALARM_DISTANCE, FAR_DISTANCE, 300, 50);

    // Ensure valid range
    tempoMultiplier = constrain(tempoMultiplier, 50, 300);

  } else {
    // Too far - stop servo and silence music
    servoSpeed = 0;          // Stop servo
    tempoMultiplier = 0;     // Silent
  }
}

// ─────────────────────────────────────────────────────────────────
// CONTROL SERVO OSCILLATION (NON-BLOCKING)
// ─────────────────────────────────────────────────────────────────

void controlServo() {
  /*
   * Oscillates servo between 0° and 180° at variable speed
   * Speed determined by servoSpeed variable
   * Non-blocking: uses millis() timing
   */

  // If servo is stopped (distance too far)
  if (servoSpeed == 0) {
    // Center the servo when stopped
    if (currentServoPosition != 90) {
      fanServo.write(90);
      currentServoPosition = 90;
    }
    return;
  }

  // Check if it's time to move servo
  if (millis() - lastServoMove >= servoSpeed) {

    // Toggle between 0° and 180°
    if (servoAtLeft) {
      fanServo.write(180);  // Swing to right
      currentServoPosition = 180;
      servoAtLeft = false;
    } else {
      fanServo.write(0);    // Swing to left
      currentServoPosition = 0;
      servoAtLeft = true;
    }

    lastServoMove = millis();
  }
}

// ─────────────────────────────────────────────────────────────────
// PLAY MARIO MUSIC AT VARIABLE TEMPO (NON-BLOCKING)
// ─────────────────────────────────────────────────────────────────

void playMarioMusic() {
  /*
   * Plays Mario theme melody continuously
   * Tempo varies based on distance (closer = faster)
   * Non-blocking: tracks current note and timing
   */

  // If music is stopped (distance too far)
  if (tempoMultiplier == 0) {
    noTone(buzzerPin);
    currentNote = 0;  // Reset to beginning when stopped
    return;
  }

  // Calculate note duration based on tempo
  // Base duration: 1000ms / marioDurations[currentNote]
  // Adjusted by tempo: × 100 / tempoMultiplier
  //
  // Example: duration = 8, tempo = 200%
  //   baseDuration = 1000/8 = 125ms
  //   adjusted = 125 × 100/200 = 62.5ms (2x faster!)

  int baseDuration = 1000 / marioDurations[currentNote];
  int adjustedDuration = (baseDuration * 100) / tempoMultiplier;

  // Check if current note has finished playing
  if (millis() - lastNoteTime >= adjustedDuration) {

    // Stop current note
    noTone(buzzerPin);

    // Move to next note
    currentNote++;

    // Loop back to beginning if we've reached the end
    if (currentNote >= melodyLength) {
      currentNote = 0;
    }

    // Play the new note
    if (marioMelody[currentNote] == REST) {
      // Silence (no tone)
      noTone(buzzerPin);
    } else {
      // Play the note
      tone(buzzerPin, marioMelody[currentNote]);
    }

    // Record when we started this note
    lastNoteTime = millis();
  }
}

// ─────────────────────────────────────────────────────────────────
// UPDATE LCD DISPLAY
// ─────────────────────────────────────────────────────────────────

void updateLCD(int dist) {
  /*
   * Updates LCD with current distance, tempo, and servo speed
   * Row 0: Distance in cm
   * Row 1: Tempo (%) and Servo Speed (ms)
   */

  // ┌─────────────────────────────────────────────────────────┐
  // │ Row 0: Distance                                         │
  // └─────────────────────────────────────────────────────────┘

  lcd.setCursor(0, 0);

  if (dist >= MAX_DISTANCE) {
    lcd.print("Dist: --  SAFE  ");
  } else {
    lcd.print("Dist: ");
    if (dist < 100) lcd.print(" ");  // Padding for alignment
    if (dist < 10) lcd.print(" ");
    lcd.print(dist);
    lcd.print("cm");

    // Add status indicator
    if (dist < ALARM_DISTANCE) {
      lcd.print(" MAX");
    } else {
      lcd.print("    ");
    }
  }

  // ┌─────────────────────────────────────────────────────────┐
  // │ Row 1: Tempo and Servo Speed                            │
  // └─────────────────────────────────────────────────────────┘

  lcd.setCursor(0, 1);

  if (tempoMultiplier == 0) {
    lcd.print("SAFE - Stopped  ");
  } else {
    // Display tempo percentage
    lcd.print("T:");
    if (tempoMultiplier < 100) lcd.print(" ");
    lcd.print(tempoMultiplier);
    lcd.print("%");

    // Display servo speed
    lcd.print(" S:");
    if (servoSpeed < 100) lcd.print(" ");
    lcd.print(servoSpeed);
    lcd.print("ms");
  }

  // ┌─────────────────────────────────────────────────────────┐
  // │ Serial Monitor Output (for debugging)                  │
  // └─────────────────────────────────────────────────────────┘

  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print("cm | Tempo: ");
  Serial.print(tempoMultiplier);
  Serial.print("% | ServoSpeed: ");
  Serial.print(servoSpeed);
  Serial.print("ms | Note: ");
  Serial.print(currentNote);
  Serial.print("/");
  Serial.println(melodyLength);
}

// ═══════════════════════════════════════════════════════════════
// 📚 LEARNING NOTES
// ═══════════════════════════════════════════════════════════════

/*
 * KEY CONCEPTS DEMONSTRATED:
 *
 * 1. VARIABLE TEMPO MUSIC PLAYBACK
 *    - Base tempo: normal note durations from array
 *    - Tempo multiplier: 50% (slow) to 300% (3x fast)
 *    - Formula: adjustedDuration = (baseDuration × 100) / tempo
 *    - Example: 100ms at 200% tempo = 50ms (2x faster!)
 *
 * 2. NON-BLOCKING MUSIC PLAYER
 *    - Tracks current note position in melody
 *    - Uses millis() to check when to advance to next note
 *    - Allows distance measurement and servo control simultaneously
 *    - Music continues smoothly while doing other tasks
 *
 * 3. MULTI-PARAMETER ADAPTIVE SYSTEM
 *    - Single input (distance) controls TWO outputs:
 *      * Servo oscillation speed (150-600ms)
 *      * Music playback tempo (50%-300%)
 *    - Both scale proportionally together
 *    - Creates cohesive adaptive experience
 *
 * 4. TEMPO vs PITCH
 *    - Tempo: How FAST notes are played (speed)
 *    - Pitch: How HIGH/LOW notes sound (frequency)
 *    - This code changes TEMPO (same notes, different speed)
 *    - Previous version changed PITCH (different frequencies)
 *
 * 5. MELODY LOOPING
 *    - When melody reaches end, resets to beginning
 *    - Creates continuous background music
 *    - Seamless transition between loops
 *
 * 6. PERCENTAGE-BASED SCALING
 *    - 100% = normal speed (reference point)
 *    - 200% = 2x faster (half duration)
 *    - 50% = 2x slower (double duration)
 *    - Intuitive for understanding speed changes
 *
 * REAL-WORLD APPLICATIONS:
 * - Adaptive alarm systems with escalating alerts
 * - Interactive museum exhibits (sound responds to visitors)
 * - Video games with dynamic music (faster when in danger)
 * - Exercise equipment (music speeds up with intensity)
 * - Parking sensors with audio feedback
 * - Proximity-based interactive art installations
 *
 * MUSIC THEORY:
 * - Tempo typically measured in BPM (beats per minute)
 * - Here we use percentage for easier calculation
 * - Each note duration scales by same factor
 * - Maintains melody recognizability at all speeds
 * - Very fast tempo creates "chipmunk effect"
 *
 * TECHNICAL DETAILS:
 * - Melody array: 48 notes total
 * - Duration array: maps to note lengths
 * - Non-blocking playback: ~20ms time resolution
 * - Distance sensing: 50ms interval (20Hz)
 * - LCD updates: 200ms interval (5Hz)
 *
 * NEXT LEVEL ENHANCEMENTS:
 * - Add multiple songs to cycle through
 * - Pitch shift AND tempo change together
 * - Reverse playback when moving away
 * - Add sound effects at specific distances
 * - MIDI output for external speakers
 * - Save favorite tempo/speed combinations
 */

// ═══════════════════════════════════════════════════════════════
// END OF CODE
// ═══════════════════════════════════════════════════════════════
