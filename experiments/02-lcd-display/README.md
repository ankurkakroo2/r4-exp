# Experiment 02: LCD Display - Show Your Name

## What We're Building

A program that displays "Ankur" (your name) on an LCD screen, along with some animations and features.

---

## 🎓 **Part 1: Understanding LCD Displays**

### What is an LCD Display?

**LCD = Liquid Crystal Display**

Think of it as a **tiny terminal/console for your Arduino**.

**Common LCD Types in Arduino Kits:**
- **16x2 LCD** - 16 characters wide, 2 rows (most common)
- **20x4 LCD** - 20 characters wide, 4 rows

Each character position can show:
- Letters: A-Z, a-z
- Numbers: 0-9
- Symbols: !@#$%^&*()
- Custom characters (you can design your own!)

### How It Works (The Basics)

**Traditional LCD Connection (Parallel Mode):**
- Uses **16 pins** - that's a LOT of wiring!
- Pins for: data (8 pins), control (3 pins), power (2 pins), backlight (2 pins), contrast (1 pin)

**Problem:** You'd use up most of your Arduino pins just for the display!

**Solution:** **I2C Module** (I2C = Inter-Integrated Circuit)

---

## 🔌 **Part 2: I2C Protocol - The Smart Way**

### What is I2C?

I2C is a **communication protocol** - think of it like USB or Ethernet, but for microcontrollers.

**The Magic:**
- Only uses **2 wires** for communication (plus 2 for power)
- Can connect **multiple devices** on the same 2 wires
- Each device has a unique **address** (like IP addresses)

**I2C Pins on Arduino:**
- **SDA** (Serial Data) - Data transmission line
- **SCL** (Serial Clock) - Clock signal to synchronize

**On Arduino Uno R4:**
- SDA = Pin A4
- SCL = Pin A5
- (Also has dedicated SDA/SCL pins near AREF)

### I2C Backpack Module

Your LCD likely has a small green/blue board soldered to the back - that's the **I2C adapter**.

**What it does:**
- Converts 2-wire I2C → 16-wire parallel LCD control
- Has a **potentiometer** (blue screw) to adjust contrast
- Makes wiring super simple!

**Common I2C Addresses:**
- **0x27** (most common)
- **0x3F** (alternative)

If one doesn't work, try the other!

---

## 🛠️ **Part 3: Wiring the LCD**

### Components Needed:
- Arduino Uno R4
- 16x2 LCD with I2C backpack
- 4 jumper wires (Female-to-Male or Male-to-Male depending on your LCD)

### Wiring Diagram:

```
LCD I2C Module          Arduino Uno R4
─────────────────       ──────────────
GND         ─────────── GND
VCC         ─────────── 5V
SDA         ─────────── A4 (or SDA pin)
SCL         ─────────── A5 (or SCL pin)
```

**Just 4 wires!** That's the beauty of I2C.

### Physical Connection:

```
Arduino                    LCD I2C Backpack
┌──────────┐              ┌────────────────┐
│          │              │ GND VCC SDA SCL│
│      GND ├──────────────┤ •              │
│       5V ├──────────────┤     •          │
│      SDA ├──────────────┤         •      │
│      SCL ├──────────────┤             •  │
│          │              │                │
│          │              │  [LCD Screen]  │
└──────────┘              └────────────────┘
```

**Wire Colors (Common Convention):**
- **Black/Brown** = GND (Ground)
- **Red** = VCC/5V (Power)
- **Yellow/Green** = SDA (Data)
- **Blue/White** = SCL (Clock)

---

## 💡 **Part 4: How the Code Works**

### The Library: LiquidCrystal_I2C

Just like `npm install` or `pip install`, Arduino has libraries.

**What it does:**
- Handles all the low-level I2C communication
- Provides simple functions like `lcd.print()` (like `cout` or `console.log`)
- Manages cursor position, screen clearing, etc.

### Key Functions:

```cpp
// Initialize LCD (address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Setup
lcd.init();              // Initialize the LCD
lcd.backlight();         // Turn on backlight (make it visible)

// Display text
lcd.setCursor(0, 0);     // Set cursor to column 0, row 0
lcd.print("Hello");      // Print text at cursor position

// Control
lcd.clear();             // Clear entire screen
lcd.noBacklight();       // Turn off backlight
lcd.backlight();         // Turn on backlight
```

### Cursor Position Explained:

```
16x2 LCD:

Column:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
       ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
Row 0: │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │
       ├──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┤
Row 1: │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │
       └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘

lcd.setCursor(0, 0);  // Top-left corner
lcd.setCursor(5, 0);  // Row 0, 6th character
lcd.setCursor(0, 1);  // Bottom-left corner
lcd.setCursor(15, 1); // Bottom-right corner
```

**Mental Model:** It's like a text editor with a fixed-width font, 16 chars wide, 2 lines tall.

---

## 🎨 **Part 5: What the Program Does**

Our program will:

1. **Display your name centered on Row 0**
2. **Show a greeting message on Row 1**
3. **Add a scrolling animation**
4. **Demonstrate cursor control**

### The Code Flow:

```cpp
setup() {
  1. Initialize LCD
  2. Turn on backlight
  3. Show welcome message
}

loop() {
  1. Display "Ankur" centered
  2. Show greeting below
  3. Wait 2 seconds
  4. Scroll text animation
  5. Repeat
}
```

---

## 🔧 **Part 6: Common Issues & Troubleshooting**

### Problem 1: Nothing shows on screen
**Causes:**
- Backlight not on → Check `lcd.backlight()`
- Wrong I2C address → Try 0x3F instead of 0x27
- Contrast too low → Adjust potentiometer on I2C module
- Wiring wrong → Double-check SDA/SCL connections

**Fix:**
- Adjust the small blue potentiometer screw on the back (contrast)
- Use I2C scanner to find correct address

### Problem 2: Garbage characters
**Causes:**
- Wrong I2C address
- Loose wire connections
- Incorrect LCD dimensions in code

**Fix:**
- Try different address (0x27 or 0x3F)
- Check all 4 wire connections are secure

### Problem 3: Backlight on but no text
**Causes:**
- Contrast set to extreme
- LCD not initialized properly

**Fix:**
- Adjust contrast potentiometer slowly
- Add delay after `lcd.init()`

---

## 🧪 **Part 7: I2C Address Scanner**

If you're not sure what address your LCD uses, we can scan for it:

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Scanner");
}

void loop() {
  for(byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  delay(5000);
}
```

---

## 📚 **What You'll Learn**

1. **I2C Communication** - Industry-standard protocol
2. **Display Control** - Cursor positioning, text formatting
3. **Libraries** - Using external code (like npm packages)
4. **Text Centering** - String manipulation in embedded systems
5. **Non-blocking Animation** - Using `millis()` for timing

---

## 🚀 **Extensions You Can Try**

Once the basic program works:

1. **Show sensor data** - Temperature, light level
2. **Scrolling text** - Marquee effect
3. **Custom characters** - Design your own icons
4. **Menu system** - Navigate with buttons
5. **Real-time clock** - Show time/date
6. **Animation frames** - Simple sprite animations

---

**Let's wire it up and make it work!**
