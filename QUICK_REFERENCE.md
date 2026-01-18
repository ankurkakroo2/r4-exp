# Arduino Quick Reference - Cheat Sheet

## Basic Sketch Structure

```cpp
// Global variables
int ledPin = 13;

void setup() {
  // Runs once on startup
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Runs forever
  digitalWrite(ledPin, HIGH);
  delay(1000);
}
```

---

## Pin Configuration

```cpp
pinMode(pin, OUTPUT);      // Set pin as output
pinMode(pin, INPUT);       // Set pin as input
pinMode(pin, INPUT_PULLUP); // Input with internal pull-up resistor
```

---

## Digital I/O

```cpp
// WRITE (Output)
digitalWrite(pin, HIGH);   // Set pin to 5V (ON)
digitalWrite(pin, LOW);    // Set pin to 0V (OFF)

// READ (Input)
int value = digitalRead(pin);  // Returns HIGH or LOW
```

---

## Analog I/O

```cpp
// READ (Input) - 10-bit ADC
int value = analogRead(A0);  // Returns 0-1023 (0V to 5V)

// WRITE (Output) - PWM
analogWrite(pin, value);     // value: 0-255 (duty cycle)
// Only works on PWM pins: ~3, ~5, ~6, ~9, ~10, ~11
```

---

## Serial Communication (Debugging)

```cpp
void setup() {
  Serial.begin(9600);  // Initialize serial at 9600 baud
}

void loop() {
  Serial.print("Value: ");      // Print without newline
  Serial.println(123);           // Print with newline
  Serial.println("Hello");       // Print string
  Serial.println(val, DEC);      // Decimal
  Serial.println(val, HEX);      // Hexadecimal
  Serial.println(val, BIN);      // Binary

  // Read from serial
  if (Serial.available() > 0) {
    char incoming = Serial.read();
  }
}
```

**View output:** `arduino-cli monitor -p /dev/cu.usbmodem2101`

---

## Timing

```cpp
delay(1000);              // Pause for 1000ms (BLOCKING)
delayMicroseconds(100);   // Pause for 100μs

unsigned long time = millis();       // Time since boot (ms)
unsigned long time = micros();       // Time since boot (μs)
```

### Non-Blocking Timing Pattern

```cpp
unsigned long previousMillis = 0;
const long interval = 1000;

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Do something every 1000ms
  }
}
```

---

## Math & Mapping

```cpp
// Map value from one range to another
long mapped = map(value, fromLow, fromHigh, toLow, toHigh);
// Example: map(512, 0, 1023, 0, 255) → 127

// Constrain value to range
int constrained = constrain(value, min, max);

// Min/Max
int minimum = min(x, y);
int maximum = max(x, y);

// Absolute value
int absolute = abs(x);

// Power
double result = pow(base, exponent);

// Square root
double result = sqrt(x);

// Random
long random(max);           // 0 to max-1
long random(min, max);      // min to max-1
randomSeed(analogRead(0));  // Seed random generator
```

---

## Data Types

```cpp
boolean   // true or false
byte      // 0 to 255 (8-bit unsigned)
char      // -128 to 127 (8-bit)
int       // -32,768 to 32,767 (16-bit)
unsigned int   // 0 to 65,535 (16-bit)
long      // -2,147,483,648 to 2,147,483,647 (32-bit)
unsigned long  // 0 to 4,294,967,295 (32-bit)
float     // -3.4028235E38 to 3.4028235E38 (32-bit)
double    // Same as float on Arduino

// Arrays
int myArray[6];
int myArray[] = {1, 2, 3, 4, 5, 6};

// Strings
char str1[] = "Hello";
String str2 = "World";  // String object (uses more memory)
```

---

## Control Structures

```cpp
// If/Else
if (condition) {
  // code
} else if (condition2) {
  // code
} else {
  // code
}

// For loop
for (int i = 0; i < 10; i++) {
  // code
}

// While loop
while (condition) {
  // code
}

// Do-While
do {
  // code
} while (condition);

// Switch
switch (variable) {
  case 1:
    // code
    break;
  case 2:
    // code
    break;
  default:
    // code
}
```

---

## Common Functions

```cpp
// Character functions
isAlpha(c)       // Is alphabetic
isDigit(c)       // Is digit
isSpace(c)       // Is whitespace

// String functions
strlen(str)      // Length
strcmp(str1, str2)   // Compare
strcpy(dest, src)    // Copy
strcat(dest, src)    // Concatenate

// Type conversion
int(x)           // Convert to int
char(x)          // Convert to char
byte(x)          // Convert to byte
long(x)          // Convert to long
float(x)         // Convert to float
```

---

## Bitwise Operations

```cpp
x & y    // AND
x | y    // OR
x ^ y    // XOR
~x       // NOT
x << y   // Shift left
x >> y   // Shift right

// Set bit
value |= (1 << bitPosition);

// Clear bit
value &= ~(1 << bitPosition);

// Toggle bit
value ^= (1 << bitPosition);

// Check bit
bool isSet = (value >> bitPosition) & 1;
```

---

## Interrupts

```cpp
// Attach interrupt
attachInterrupt(digitalPinToInterrupt(pin), ISR_function, mode);
// mode: LOW, CHANGE, RISING, FALLING

// Detach interrupt
detachInterrupt(digitalPinToInterrupt(pin));

// Interrupt Service Routine (ISR)
void ISR_function() {
  // Keep this SHORT and FAST
  // No Serial.print(), delay(), etc.
  // Just set flags or variables
}

// Example
volatile bool flag = false;

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), buttonISR, FALLING);
}

void buttonISR() {
  flag = true;  // Just set a flag
}

void loop() {
  if (flag) {
    // Handle the interrupt here
    flag = false;
  }
}
```

---

## Pin Constants (UNO R4 Minima)

```cpp
LED_BUILTIN  // Built-in LED (usually pin 13)

// Digital pins: 0-13
// Analog pins: A0-A5 (can also be digital 14-19)
// PWM pins: 3, 5, 6, 9, 10, 11 (marked with ~)

// Special pins
// 0, 1: Serial TX/RX (avoid if using Serial)
// 2, 3: External interrupts
// SDA, SCL: I2C communication
// 10-13: SPI communication (SS, MOSI, MISO, SCK)
```

---

## Common Libraries

### Servo Motor
```cpp
#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9);  // Attach to pin 9
}

void loop() {
  myServo.write(90);  // Move to 90 degrees
  delay(1000);
}
```

### LCD Display (I2C)
```cpp
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address, columns, rows

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);  // Column, row
  lcd.print("Hello!");
}
```

### DHT Temperature Sensor
```cpp
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();     // Celsius
  float humidity = dht.readHumidity();
}
```

### Ultrasonic Sensor (HC-SR04)
```cpp
const int trigPin = 9;
const int echoPin = 10;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // cm
  return distance;
}
```

---

## Resistor Color Codes

```
Black   = 0
Brown   = 1
Red     = 2
Orange  = 3
Yellow  = 4
Green   = 5
Blue    = 6
Violet  = 7
Gray    = 8
White   = 9

Gold    = ±5% tolerance
Silver  = ±10% tolerance

Example: Brown-Black-Red = 10 × 10² = 1000Ω (1kΩ)
Example: Red-Red-Brown  = 22 × 10¹ = 220Ω
```

---

## Common Circuit Patterns

### LED with Resistor
```
Pin → [Resistor 220Ω] → [LED +|-] → GND
```

### Button with Pull-down
```
Pin → [Button] → 5V
Pin → [Resistor 10kΩ] → GND
```

### Button with Pull-up (Internal)
```cpp
pinMode(buttonPin, INPUT_PULLUP);
// No external resistor needed!
// Button pressed = LOW
```

### Potentiometer
```
5V → [Pot] → GND
     Middle pin → Analog Pin
```

---

## Debugging Tips

1. **Serial Monitor is your console.log()**
   ```cpp
   Serial.println(variable);
   ```

2. **Blink patterns for debugging**
   ```cpp
   // 1 blink = error case 1
   // 2 blinks = error case 2
   ```

3. **Test components separately**
   - One LED first, then all LEDs
   - One sensor first, then combined

4. **Check connections**
   - Power rails (red = 5V, blue = GND)
   - Loose wires?
   - Correct pins?

5. **Use multimeter**
   - Check voltage on pins
   - Check continuity

---

## Common Mistakes

❌ **Forgetting pinMode()**
```cpp
digitalWrite(13, HIGH);  // Won't work without pinMode!
```

✅ **Always set pinMode first**
```cpp
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);
```

---

❌ **LED without resistor**
```
Pin → LED → GND  // LED will burn out!
```

✅ **Always use resistor**
```
Pin → 220Ω → LED → GND
```

---

❌ **Using delay() everywhere**
```cpp
delay(1000);  // Blocks everything!
```

✅ **Use millis() for non-blocking**
```cpp
if (millis() - lastTime > 1000) {
  // Do something
}
```

---

❌ **Not checking Serial.available()**
```cpp
char c = Serial.read();  // May read garbage
```

✅ **Check before reading**
```cpp
if (Serial.available() > 0) {
  char c = Serial.read();
}
```

---

## Memory Optimization

```cpp
// Use F() macro for strings to save RAM
Serial.println(F("This uses Flash memory, not RAM"));

// Use PROGMEM for constants
const char string[] PROGMEM = "Stored in Flash";

// Use byte instead of int when possible
byte ledPin = 13;  // 1 byte instead of 2
```

---

## Quick Board Info

**Arduino UNO R4 Minima**
- Flash: 256 KB (program storage)
- SRAM: 32 KB (runtime variables)
- EEPROM: 8 KB (persistent storage)
- Clock: 48 MHz
- Operating Voltage: 5V
- Input Voltage: 6-24V (via barrel jack or Vin)

---

## Useful Links

- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Arduino Forum](https://forum.arduino.cc/)
- [Arduino Project Hub](https://create.arduino.cc/projecthub)

---

**Print this and keep it handy while coding!**
