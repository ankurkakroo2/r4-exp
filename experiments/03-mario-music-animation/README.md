# Experiment 03: Super Mario Bros - Music + Walking Animation

## What We're Building

A multi-component project that:
1. 🎵 Plays the **Super Mario Bros theme song** on the buzzer
2. 🏃 Shows **Mario walking** across the LCD screen
3. 🎭 Uses **custom pixel art** characters
4. ⚡ Synchronizes music and animation

---

## 🎓 New Concepts You'll Learn

### 1. **LCD Custom Characters (Pixel Art!)**

**What it is:**
- LCD can store up to **8 custom 5×8 pixel characters**
- Each character is like a tiny sprite (5 pixels wide, 8 pixels tall)
- You design the pattern with binary/hex values

**How it works:**

```
Each character = 5×8 grid of pixels

Example: Simple Mario head
  ▓▓▓▓▓
  ▓   ▓
  ▓ • •▓
  ▓   ▓
  ▓▓▓▓▓

Binary representation:
11111 = 31
10001 = 17
10101 = 21
10001 = 17
11111 = 31
```

**Creating custom characters:**
```cpp
byte marioWalk1[8] = {
  0b00000,  // Row 0: .....
  0b01110,  // Row 1: .███.
  0b01110,  // Row 2: .███.
  0b00100,  // Row 3: ..█..
  0b01110,  // Row 4: .███.
  0b10101,  // Row 5: █.█.█
  0b00100,  // Row 6: ..█..
  0b01010,  // Row 7: .█.█.
};

lcd.createChar(0, marioWalk1);  // Store in slot 0
lcd.write(0);                    // Display character 0
```

**Think of it like:**
- Each character slot = Emoji/sprite
- You design the pixel pattern
- Then display it anywhere on LCD

---

### 2. **Passive Buzzer - Playing Music**

**Passive vs Active Buzzer:**

| Active Buzzer | Passive Buzzer |
|---------------|----------------|
| Fixed tone only | Variable frequency |
| Like a doorbell | Like a speaker |
| `digitalWrite()` | `tone()` function |
| Can't play melodies | CAN play melodies ✓ |

**How tone() works:**
```cpp
tone(pin, frequency, duration);

// Play A note (440Hz) for 500ms
tone(buzzerPin, 440, 500);

// Play C note (262Hz) for 1 second
tone(buzzerPin, 262, 1000);
```

**Musical Notes = Frequencies:**
```
C  = 262 Hz
D  = 294 Hz
E  = 330 Hz
F  = 349 Hz
G  = 392 Hz
A  = 440 Hz
B  = 494 Hz
C' = 523 Hz (one octave higher)
```

**Playing a melody:**
```cpp
int melody[] = {262, 294, 330, 349, 392};  // C D E F G
int durations[] = {4, 4, 4, 4, 2};         // Quarter notes and half note

for (int i = 0; i < 5; i++) {
  tone(buzzerPin, melody[i], 1000/durations[i]);
  delay(1000/durations[i] * 1.3);  // Pause between notes
}
```

---

### 3. **Animation on LCD**

**How to create walking effect:**

1. **Design multiple frames** (like flipbook animation)
   ```
   Frame 1: Mario legs together
   Frame 2: Mario left leg forward
   Frame 3: Mario legs together
   Frame 4: Mario right leg forward
   ```

2. **Display frames rapidly**
   ```cpp
   lcd.write(0);  // Frame 1
   delay(200);
   lcd.setCursor(pos, 1);
   lcd.write(1);  // Frame 2
   delay(200);
   ```

3. **Move position across screen**
   ```cpp
   for (int pos = 0; pos < 16; pos++) {
     lcd.setCursor(pos, 1);  // Move right
     lcd.write(0);            // Display Mario
     delay(300);
     lcd.setCursor(pos, 1);
     lcd.print(" ");          // Clear old position
   }
   ```

---

### 4. **Multi-tasking (Music + Animation)**

**Challenge:** How to play music AND animate simultaneously?

**Problem:**
```cpp
// This BLOCKS everything for 3 seconds!
tone(buzzer, 440, 3000);
delay(3000);

// Animation can't happen during this time!
```

**Solution 1: Short tones**
```cpp
// Play short notes, animate between them
tone(buzzer, 440, 200);
delay(200);
updateMario();  // Move Mario during gap

tone(buzzer, 494, 200);
delay(200);
updateMario();  // Move Mario again
```

**Solution 2: Non-blocking with millis()**
```cpp
unsigned long lastMusicUpdate = 0;
unsigned long lastAnimUpdate = 0;

void loop() {
  if (millis() - lastMusicUpdate > musicInterval) {
    playNextNote();
    lastMusicUpdate = millis();
  }

  if (millis() - lastAnimUpdate > animInterval) {
    updateMario();
    lastAnimUpdate = millis();
  }
}
```

---

## 🎮 The Super Mario Theme

**Musical structure:**
```
Main Theme Intro:
E E . E . C E . G . . . G . . .

Notes in Hz:
E  = 659 Hz
C  = 523 Hz
G  = 784 Hz (high)
G  = 392 Hz (low)
```

**Timing:**
- Quarter note (♩) = 150ms
- Eighth note (♪) = 75ms
- Dot (.) = Rest/pause

---

## 🏃 Mario Walking Animation Design

**Mario Sprite (simplified 5×8 pixel art):**

```
Frame 1: Standing/Left foot
  . █ █ █ .      0b01110
  . █ █ █ .      0b01110
  . . █ . .      0b00100
  . █ █ █ .      0b01110
  █ . █ . █      0b10101
  . . █ . .      0b00100
  . █ . █ .      0b01010
  . . . . .      0b00000

Frame 2: Walking/Right foot
  . █ █ █ .      0b01110
  . █ █ █ .      0b01110
  . . █ . .      0b00100
  . █ █ █ .      0b01110
  █ . █ . █      0b10101
  . . █ . .      0b00100
  . . █ . █      0b00101
  . . . . .      0b00000
```

**Alternative: Super simple Mario**
```
   ███     (Hat)
   ███     (Face)
    █      (Body)
   ███     (Arms)
  █ █ █    (Legs)
   █
  █ █      (Feet)
```

---

## 🔌 Wiring

### Components Needed:
1. Arduino Uno R4 ✓
2. 16×2 LCD (I2C) ✓ (already connected)
3. **Passive Buzzer** (NEW!)
4. 1 jumper wire for buzzer

### New Connection:

```
Passive Buzzer          Arduino
──────────────          ───────
Positive (+)      →     Pin 8
Negative (-)      →     GND
```

**How to identify Passive vs Active buzzer:**
- **Look at the bottom:**
  - Green PCB with components = Passive ✓
  - Sealed black with sticker = Active
- **Or test:**
  - Connect to power: constant tone = Active
  - Connect to power: no/faint sound = Passive ✓

---

## 🎯 Program Flow

```
Setup:
├─ Initialize LCD
├─ Create Mario custom characters (2-3 frames)
├─ Setup buzzer pin
└─ Display title screen

Loop:
├─ Play Mario theme (note by note)
│  ├─ Play note on buzzer
│  └─ Short delay
│
└─ Animate Mario
   ├─ Move position left to right
   ├─ Alternate between walk frames
   └─ Clear trail behind

When song ends:
└─ Mario reaches end of screen
   └─ Loop back / Show victory screen
```

---

## 🎨 Code Architecture

```cpp
// Define musical notes (frequencies)
#define NOTE_E 659
#define NOTE_C 523
#define NOTE_G 784

// Mario custom characters
byte marioFrame1[8] = {...};
byte marioFrame2[8] = {...};

// Mario melody (note frequencies)
int melody[] = {NOTE_E, NOTE_E, 0, NOTE_E, ...};

// Note durations (4=quarter, 8=eighth)
int noteDurations[] = {4, 4, 8, 4, ...};

void setup() {
  lcd.createChar(0, marioFrame1);
  lcd.createChar(1, marioFrame2);
}

void loop() {
  playMarioTheme();
  animateMarioWalking();
}
```

---

## 🔧 Technical Challenges & Solutions

### Challenge 1: Smooth Animation
**Problem:** Flickering when clearing/redrawing Mario

**Solution:**
```cpp
// Clear only the old position, not whole screen
lcd.setCursor(oldPos, 1);
lcd.print(" ");

// Draw at new position
lcd.setCursor(newPos, 1);
lcd.write(0);
```

### Challenge 2: Sync Music & Animation
**Problem:** Music blocks animation or vice versa

**Solution:**
```cpp
// Interleave: play note, move mario, play note, move mario
for (int i = 0; i < melodyLength; i++) {
  tone(buzzer, melody[i], duration[i]);
  animateOneStep();  // Move mario during note
  delay(duration[i]);
}
```

### Challenge 3: Memory Limits
**Problem:** Large melody arrays consume RAM

**Solution:**
```cpp
// Store in Flash memory (PROGMEM)
const int melody[] PROGMEM = {NOTE_E, NOTE_E, ...};

// Read from Flash when needed
int note = pgm_read_word(&melody[i]);
```

---

## 🎵 Super Mario Theme - Full Melody

**Main intro (what we'll play):**
```
E  E  -  E  -  C  E  -  G  -  -  -  G  -  -  -
C  -  -  G  -  -  E  -  -  A  -  B  -  A# A  -
G  E  G  A  -  F  G  -  E  -  C  D  B  -  -  -
```

**In frequencies:**
```cpp
{659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 0, 0, 392, 0, 0, 0,
 523, 0, 0, 392, 0, 0, 659, 0, 0, 880, 0, 988, 0, 932, 880, 0,
 784, 659, 784, 880, 0, 698, 784, 0, 659, 0, 523, 587, 494, 0, 0, 0}
```

---

## 🚀 Expected Result

When you run the program:

1. **Title screen appears:**
   ```
   ┌────────────────┐
   │ Super Mario!   │
   │                │
   └────────────────┘
   ```

2. **Music starts playing** (Mario theme)

3. **Mario appears and walks:**
   ```
   ┌────────────────┐
   │ Super Mario!   │
   │   🏃          │  ← Mario animating
   └────────────────┘
   ```

4. **Mario walks across screen** (left to right)
   - Legs alternate (walking animation)
   - Music plays in sync
   - Reaches end of screen

5. **Loop repeats or shows victory!**

---

## 🎓 What You'll Learn

1. **Custom LCD Characters** - Pixel art on LCD
2. **Tone Generation** - Making music with buzzer
3. **Animation** - Frame-by-frame movement
4. **Multi-tasking** - Music + animation simultaneously
5. **Arrays & Loops** - Managing melodies and timing
6. **Creative Programming** - Making fun, engaging projects!

---

## 🎮 Extensions You Can Add

After the basic version works:

1. **Coins** - Mario collects coins across screen
2. **Jump** - Mario jumps with button press
3. **Score display** - Count coins on LCD
4. **Multiple songs** - Different tunes
5. **Enemies** - Goomba appears randomly
6. **Power-ups** - Mushroom makes Mario bigger
7. **Game over screen** - When Mario hits enemy

---

**Ready to code Mario? Let's make it happen! 🍄**
