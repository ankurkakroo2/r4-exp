/*
 * ═══════════════════════════════════════════════════════════════
 *  EXPERIMENT 03: Super Mario Bros - Music + Walking Animation
 * ═══════════════════════════════════════════════════════════════
 *
 *  WHAT THIS DOES:
 *  - Plays Super Mario Bros theme on passive buzzer
 *  - Shows Mario walking across LCD screen
 *  - Uses custom 5×8 pixel characters
 *  - Synchronizes music and animation
 *
 *  HARDWARE NEEDED:
 *  - Arduino Uno R4
 *  - 16x2 LCD with I2C backpack (already connected)
 *  - Passive buzzer (NEW!)
 *  - 1 jumper wire for buzzer
 *
 *  NEW CONNECTION:
 *  Buzzer (+)  → Arduino Pin 8
 *  Buzzer (-)  → Arduino GND
 *
 * ═══════════════════════════════════════════════════════════════
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ─────────────────────────────────────────────────────────────────
// HARDWARE CONFIGURATION
// ─────────────────────────────────────────────────────────────────

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD at address 0x27
const int buzzerPin = 8;              // Passive buzzer on pin 8

// ─────────────────────────────────────────────────────────────────
// MUSICAL NOTES (Frequencies in Hz)
// ─────────────────────────────────────────────────────────────────

/*
 * Each musical note has a specific frequency
 * These are the notes from the C major scale
 */

#define NOTE_C4  262   // C (Do)
#define NOTE_CS4 277   // C# (Do sharp)
#define NOTE_D4  294   // D (Re)
#define NOTE_DS4 311   // D# (Re sharp)
#define NOTE_E4  330   // E (Mi)
#define NOTE_F4  349   // F (Fa)
#define NOTE_FS4 370   // F# (Fa sharp)
#define NOTE_G4  392   // G (Sol) - Low G
#define NOTE_GS4 415   // G# (Sol sharp)
#define NOTE_A4  440   // A (La)
#define NOTE_AS4 466   // A# (La sharp)
#define NOTE_B4  494   // B (Si)
#define NOTE_C5  523   // C one octave higher
#define NOTE_D5  587   // D one octave higher
#define NOTE_E5  659   // E one octave higher
#define NOTE_F5  698   // F one octave higher
#define NOTE_G5  784   // G one octave higher (High G)
#define NOTE_A5  880   // A one octave higher
#define NOTE_AS5 932   // A# one octave higher
#define NOTE_B5  988   // B one octave higher

#define REST     0     // Silence (no tone)

// ─────────────────────────────────────────────────────────────────
// SUPER MARIO THEME MELODY
// ─────────────────────────────────────────────────────────────────

/*
 * The famous Super Mario Bros intro theme!
 * Each number is a note frequency
 * 0 = REST (silence)
 */

int marioMelody[] = {
  NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5, REST,
  NOTE_G5, REST, REST, REST, NOTE_G4, REST, REST, REST,

  NOTE_C5, REST, REST, NOTE_G4, REST, REST, NOTE_E4, REST,
  REST, NOTE_A4, REST, NOTE_B4, REST, NOTE_AS4, NOTE_A4, REST,

  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, REST, NOTE_F5, NOTE_G5, REST,
  NOTE_E5, REST, NOTE_C5, NOTE_D5, NOTE_B4, REST, REST, REST
};

/*
 * Note durations: 4 = quarter note, 8 = eighth note, etc.
 * Smaller number = longer note
 */
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
// MARIO CUSTOM CHARACTERS (Pixel Art!)
// ─────────────────────────────────────────────────────────────────

/*
 * Each character is 5 pixels wide × 8 pixels tall
 * 0b = binary notation
 * 1 = filled pixel, 0 = empty pixel
 *
 * We create 2 frames for walking animation
 */

// Mario Frame 1: Left foot forward
byte marioFrame1[8] = {
  0b01110,  //  .███.   (Hat)
  0b01110,  //  .███.   (Face)
  0b00100,  //  ..█..   (Body)
  0b01110,  //  .███.   (Arms out)
  0b10101,  //  █.█.█   (Legs apart)
  0b00100,  //  ..█..   (Body)
  0b01010,  //  .█.█.   (Left foot forward)
  0b01000   //  .█...   (Left foot)
};

// Mario Frame 2: Right foot forward
byte marioFrame2[8] = {
  0b01110,  //  .███.   (Hat)
  0b01110,  //  .███.   (Face)
  0b00100,  //  ..█..   (Body)
  0b01110,  //  .███.   (Arms out)
  0b10101,  //  █.█.█   (Legs apart)
  0b00100,  //  ..█..   (Body)
  0b01010,  //  .█.█.   (Right foot forward)
  0b00010   //  ...█.   (Right foot)
};

// Coin character (optional decoration)
byte coinChar[8] = {
  0b00000,  //  .....
  0b01110,  //  .███.
  0b11111,  //  █████
  0b11011,  //  ██.██
  0b11111,  //  █████
  0b01110,  //  .███.
  0b00000,  //  .....
  0b00000   //  .....
};

// ─────────────────────────────────────────────────────────────────
// ANIMATION VARIABLES
// ─────────────────────────────────────────────────────────────────

int marioPosition = 0;      // Mario's X position (0-15)
int currentFrame = 0;        // Which walk frame (0 or 1)
int noteIndex = 0;           // Current note in melody

// ─────────────────────────────────────────────────────────────────
// SETUP FUNCTION
// ─────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(9600);
  Serial.println("🍄 Super Mario Bros - Starting!");

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Create custom characters
  lcd.createChar(0, marioFrame1);  // Slot 0 = Mario frame 1
  lcd.createChar(1, marioFrame2);  // Slot 1 = Mario frame 2
  lcd.createChar(2, coinChar);     // Slot 2 = Coin

  // Setup buzzer pin
  pinMode(buzzerPin, OUTPUT);

  // Show title screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Super Mario!  ");
  lcd.setCursor(0, 1);
  lcd.print("  Get Ready!   ");

  delay(2000);
  lcd.clear();

  Serial.println("Let's-a-go!");
}

// ─────────────────────────────────────────────────────────────────
// MAIN LOOP
// ─────────────────────────────────────────────────────────────────

void loop() {

  // ┌───────────────────────────────────────────────────────────┐
  // │ Play the complete Mario theme with walking animation     │
  // └───────────────────────────────────────────────────────────┘

  playMarioWithWalking();

  // ┌───────────────────────────────────────────────────────────┐
  // │ Show victory screen                                       │
  // └───────────────────────────────────────────────────────────┘

  showVictory();

  // Wait before looping
  delay(2000);

  // Reset for next loop
  marioPosition = 0;
  noteIndex = 0;
  lcd.clear();
}

// ─────────────────────────────────────────────────────────────────
// PLAY MARIO THEME WITH WALKING ANIMATION
// ─────────────────────────────────────────────────────────────────

void playMarioWithWalking() {

  /*
   * This function plays the Mario theme song
   * AND animates Mario walking at the same time!
   *
   * Strategy: Play each note, move Mario during note
   */

  // Display "Now Playing" text on top row
  lcd.setCursor(0, 0);
  lcd.print("Super Mario!    ");

  // Play each note in the melody
  for (int i = 0; i < melodyLength; i++) {

    // ┌─────────────────────────────────────────────────────────┐
    // │ 1. Play the note                                        │
    // └─────────────────────────────────────────────────────────┘

    int noteDuration = 1000 / marioDurations[i];

    if (marioMelody[i] == REST) {
      // Silence (no tone)
      noTone(buzzerPin);
    } else {
      // Play the note
      tone(buzzerPin, marioMelody[i], noteDuration);
    }

    // ┌─────────────────────────────────────────────────────────┐
    // │ 2. Move Mario while note plays                          │
    // └─────────────────────────────────────────────────────────┘

    // Move Mario every 2 notes
    if (i % 2 == 0 && marioPosition < 15) {
      moveMario();
    }

    // Wait for note to finish
    delay(noteDuration);

    // Small pause between notes (makes melody clearer)
    noTone(buzzerPin);
    delay(noteDuration * 0.3);
  }

  // Stop all sound
  noTone(buzzerPin);
}

// ─────────────────────────────────────────────────────────────────
// MOVE MARIO ONE STEP
// ─────────────────────────────────────────────────────────────────

void moveMario() {

  /*
   * Moves Mario one position to the right
   * Alternates between walking frames for animation
   */

  // Clear old position
  lcd.setCursor(marioPosition, 1);
  lcd.print(" ");

  // Move forward
  marioPosition++;

  // Check if Mario reached end
  if (marioPosition > 15) {
    marioPosition = 15;  // Stop at edge
    return;
  }

  // Alternate frames (creates walking effect)
  currentFrame = 1 - currentFrame;  // Toggles between 0 and 1

  // Draw Mario at new position
  lcd.setCursor(marioPosition, 1);
  lcd.write(currentFrame);  // Display frame 0 or frame 1

  // Debug output
  Serial.print("Mario at position: ");
  Serial.print(marioPosition);
  Serial.print(" | Frame: ");
  Serial.println(currentFrame);
}

// ─────────────────────────────────────────────────────────────────
// SHOW VICTORY SCREEN
// ─────────────────────────────────────────────────────────────────

void showVictory() {

  lcd.clear();

  // Top row: Victory message
  lcd.setCursor(0, 0);
  lcd.print("   Victory!    ");

  // Bottom row: Mario at end with coin
  lcd.setCursor(13, 1);
  lcd.write(0);  // Mario
  lcd.setCursor(15, 1);
  lcd.write(2);  // Coin

  // Play victory jingle
  tone(buzzerPin, NOTE_G5, 100);
  delay(100);
  tone(buzzerPin, NOTE_C5, 100);
  delay(100);
  tone(buzzerPin, NOTE_E5, 100);
  delay(100);
  tone(buzzerPin, NOTE_G5, 400);
  delay(400);
  noTone(buzzerPin);

  Serial.println("🏆 Level Complete!");
}

// ─────────────────────────────────────────────────────────────────
// HELPER FUNCTION: Play Single Note
// ─────────────────────────────────────────────────────────────────

void playNote(int frequency, int duration) {
  /*
   * Plays a single musical note
   *
   * frequency: Note frequency in Hz (e.g., 440 = A note)
   * duration: How long to play in milliseconds
   */

  if (frequency == REST) {
    noTone(buzzerPin);
  } else {
    tone(buzzerPin, frequency, duration);
  }

  delay(duration);
  noTone(buzzerPin);  // Stop the tone
}

// ═══════════════════════════════════════════════════════════════
// 📚 LEARNING NOTES
// ═══════════════════════════════════════════════════════════════

/*
 * KEY CONCEPTS DEMONSTRATED:
 *
 * 1. CUSTOM LCD CHARACTERS
 *    - Created 2 Mario walk frames (5×8 pixels each)
 *    - Used binary notation (0b01110) for pixel patterns
 *    - lcd.createChar(slot, pattern) stores character
 *    - lcd.write(slot) displays custom character
 *
 * 2. TONE GENERATION
 *    - tone(pin, frequency, duration) plays sound
 *    - noTone(pin) stops sound
 *    - Musical notes = specific frequencies (A = 440Hz)
 *    - Arrays store melodies and timing
 *
 * 3. ANIMATION
 *    - Frame-by-frame updates create motion
 *    - Alternating between frames creates walking effect
 *    - Clear old position, draw new position
 *    - Like flipbook animation!
 *
 * 4. SYNCHRONIZATION
 *    - Music and animation happen together
 *    - Move Mario during note playback
 *    - Timing coordination with delays
 *
 * 5. MULTI-TASKING PATTERN
 *    - for loop plays each note
 *    - Inside loop: play note + animate
 *    - Sequential execution with timing
 *
 * REAL-WORLD APPLICATIONS:
 * - Game development (sprites, sound)
 * - Music players (MP3 players, ringtones)
 * - User interfaces (animations, feedback)
 * - Entertainment systems
 * - Educational toys
 *
 * NEXT LEVEL:
 * - Add buttons for Mario jump
 * - Create enemies (Goombas)
 * - Add score counter
 * - Multiple levels
 * - Power-ups (mushroom, star)
 * - Background music tracks
 */

// ═══════════════════════════════════════════════════════════════
// END OF CODE
// ═══════════════════════════════════════════════════════════════
