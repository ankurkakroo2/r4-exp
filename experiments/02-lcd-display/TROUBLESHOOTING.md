# LCD Display Troubleshooting Guide

## Problem: Nothing Shows on Screen

### Step 1: Check Backlight

**Look at the LCD:**
- Is there a blue/white glow from behind?
- If YES → LCD is powered, go to Step 2
- If NO → Check power connections

**Fix power:**
```
1. Verify GND wire connected to Arduino GND
2. Verify VCC wire connected to Arduino 5V
3. Try different GND/5V pins on Arduino
4. Check for loose connections
```

---

### Step 2: Adjust Contrast

**The blue potentiometer on I2C backpack:**
```
     ┌─────────────┐
     │  I2C Module │
     │    [POT]    │  ← Blue screw
     └─────────────┘
```

**How to adjust:**
1. Find small blue potentiometer (screw)
2. Use small screwdriver (or fingernail)
3. Turn SLOWLY clockwise
4. Watch LCD - contrast will change
5. Stop when characters are clearest

**What you'll see:**
- Too dark: Can't see anything
- Perfect: Clear black characters
- Too light: Faint or all black squares

---

### Step 3: Check I2C Address

**Your LCD might use address 0x3F instead of 0x27**

**To check:**

1. Upload the I2C Scanner:
   ```bash
   cd /Users/ankur/D/Playground/r4-exp
   arduino-cli compile --upload -p /dev/cu.usbmodem2101 \
     --fqbn arduino:renesas_uno:minima \
     experiments/02-lcd-display/I2C_Scanner
   ```

2. Open Serial Monitor:
   ```bash
   arduino-cli monitor -p /dev/cu.usbmodem2101
   ```

3. Note the address shown (e.g., 0x27 or 0x3F)

4. Edit DisplayName.ino:
   ```cpp
   // Change this line (around line 45):
   LiquidCrystal_I2C lcd(0x27, 16, 2);  // Try 0x3F if 0x27 doesn't work
   ```

5. Re-upload the corrected code

---

### Step 4: Verify Wiring

**Check each connection:**

```
LCD Pin  →  Arduino Pin  →  Wire Color
───────────────────────────────────────
GND      →  GND          →  Black/Brown
VCC      →  5V           →  Red
SDA      →  A4           →  Yellow/Green
SCL      →  A5           →  Blue/White
```

**Common mistakes:**
- ✗ SDA and SCL swapped
- ✗ Using wrong analog pins (must be A4 and A5)
- ✗ Loose connections
- ✗ Wire not fully inserted into breadboard/header

**Test:**
1. Gently wiggle each wire
2. Push firmly into connectors
3. Try different wires if available

---

## Problem: Garbage Characters

### Cause 1: Wrong I2C Address

**You'll see:** Random squares, strange symbols, flickering

**Fix:** Use I2C Scanner (see Step 3 above)

---

### Cause 2: Loose Connection

**You'll see:** Intermittent display, characters appearing/disappearing

**Fix:**
1. Check SDA wire (data line)
2. Check SCL wire (clock line)
3. Ensure both are firmly connected
4. Try different wires

---

### Cause 3: Power Issues

**You'll see:** Dim display, random characters, resets

**Fix:**
1. Don't power too many devices from Arduino 5V
2. Use external 5V power supply if available
3. Check USB cable provides enough power

---

## Problem: Backlight On But No Text

### Cause: Contrast Too Extreme

**Fix:**
1. Locate blue potentiometer on I2C module
2. Turn it all the way clockwise
3. Slowly turn counter-clockwise
4. Watch for characters to appear
5. Fine-tune for best clarity

---

## Problem: Only Squares Visible

### This is actually normal during initialization!

**What it means:**
- LCD is powered ✓
- Contrast is good ✓
- LCD is in initialization mode

**Why it happens:**
- LCD not initialized properly
- Code not running

**Fix:**
1. Press Arduino RESET button
2. Re-upload code
3. Check Serial Monitor for debug messages:
   ```bash
   arduino-cli monitor -p /dev/cu.usbmodem2101
   ```

---

## Problem: First Row Works, Second Row Doesn't

### Cause: Wrong LCD Size in Code

**Fix:**
Edit DisplayName.ino:
```cpp
// Make sure this matches your LCD:
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 columns, 2 rows
//                           ^^  ^
//                           |   └─ Rows (should be 2)
//                           └───── Columns (16 for most LCDs)
```

If you have a 20x4 LCD, change to:
```cpp
LiquidCrystal_I2C lcd(0x27, 20, 4);
```

---

## Advanced Debugging

### Test Individual Components

**1. Test I2C Connection:**
```bash
# Upload I2C Scanner
arduino-cli compile --upload -p /dev/cu.usbmodem2101 \
  --fqbn arduino:renesas_uno:minima \
  experiments/02-lcd-display/I2C_Scanner

# Check output
arduino-cli monitor -p /dev/cu.usbmodem2101
```

**Expected output:**
```
I2C device found at address 0x27!
  → Likely device: LCD with PCF8574 I2C backpack
```

**2. Test Simple Code:**
Create minimal test:
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Test");
}

void loop() {
  // Nothing
}
```

---

## Pin Reference

### Arduino Uno R4 I2C Pins:

```
Pin A4 = SDA (Serial Data)
Pin A5 = SCL (Serial Clock)
```

**Alternative location:**
Some R4 boards have dedicated SDA/SCL pins near AREF pin.
Both locations work!

---

## LCD I2C Module Types

### Common Addresses:
- **0x27** - PCF8574T chip (most common)
- **0x3F** - PCF8574AT chip (alternative)

### Identifying Your Module:
Look at the chip on the I2C backpack:
- PCF8574T → Use 0x27
- PCF8574AT → Use 0x3F

### Jumper Configuration:
Some modules have A0/A1/A2 jumpers to change address:
- All open: 0x27
- A0 closed: 0x26
- A1 closed: 0x25
- etc.

---

## Still Not Working?

### Check These:

1. **Arduino is powered?**
   - USB connected
   - Power LED on Arduino is lit

2. **LCD has power?**
   - Backlight glowing
   - VCC and GND connected

3. **Code uploaded?**
   - "Done uploading" message shown
   - No compilation errors

4. **Right board selected?**
   ```bash
   arduino-cli board list
   # Should show Arduino UNO R4 Minima
   ```

5. **Library installed?**
   ```bash
   arduino-cli lib list | grep LiquidCrystal_I2C
   # Should show LiquidCrystal_I2C 2.0.0
   ```

---

## Quick Test Checklist

```
[ ] LCD backlight is ON
[ ] Contrast adjusted (blue screw)
[ ] All 4 wires connected firmly
[ ] SDA → A4
[ ] SCL → A5
[ ] GND → GND
[ ] VCC → 5V
[ ] Code compiled without errors
[ ] Code uploaded successfully
[ ] Tried both 0x27 and 0x3F addresses
[ ] Run I2C Scanner to verify address
```

---

## Get Help

If still stuck:

1. **Check Serial Monitor:**
   ```bash
   arduino-cli monitor -p /dev/cu.usbmodem2101
   ```
   Look for error messages

2. **Try I2C Scanner:**
   It will tell you if LCD is detected

3. **Take a photo:**
   Of your wiring and I2C backpack chip

4. **Note:**
   - What you see on LCD (nothing? squares? garbage?)
   - What I2C Scanner reports
   - Which address you tried (0x27 or 0x3F)

---

**99% of LCD issues are:**
1. Contrast not adjusted (75%)
2. Wrong I2C address (20%)
3. Loose wire connection (4%)
4. Other (1%)

**Turn that blue screw first!** 🔧
