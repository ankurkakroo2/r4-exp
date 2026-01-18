# Arduino R4 Tiny Experiments


## The Setup

**Board:** Arduino Uno R4 Minima
**Kit:** Quad Store Super Starter Kit (fancy name for "box of parts I'm gradually figuring out")

**What I'm working with:**
- HC-SR04 Ultrasonic Distance Sensor (the "eyes")
- 16x2 LCD Display (the "mouth")
- SG90 Servo Motor (the "arms")
- Buzzers - both passive and active (the "voice")
- A bunch of LEDs, resistors, and mysterious components
- Breadboard for making wiring mistakes and fixing them

## Experiments So Far

### Experiment 01: Blink
The classic "hello world" of hardware. Got an LED to blink. Felt disproportionately proud of myself.

What I learned:
- How Arduino actually runs code (setup once, loop forever)
- Making pins go high and low
- Why `delay()` is both useful and terrible
- Uploading code to actual hardware

### Experiment 02: LCD Display
Got an LCD to display text. May or may not have used it to display a romantic message. (Spoiler: I did.)

What I learned:
- I2C communication (fancy name for "talks to stuff with 2 wires")
- How to find device addresses when you don't know them
- LCD libraries that make life way easier
- Why having an I2C scanner is a lifesaver

### Experiment 03: Super Mario Bros
Mario theme music + walking animation on the LCD. This was the one that made me go "oh, I can actually make cool stuff with this."

What I learned:
- Creating custom pixel characters with binary
- Making noise with buzzers (passive vs active - who knew?)
- Timing music with animations
- Why 48 notes of melody data takes a while to debug

### Experiment 04: Proximity Alarm
Basically car parking sensors. Beeps faster as stuff gets closer. Surprisingly satisfying to annoy people with.

What I learned:
- Ultrasonic sensors are like tiny bats
- Math: distance = (time × speed of sound) / 2
- Non-blocking code (millis() > delay() - life lesson there too)
- Zone-based state machines
- That pulse width measurement is actually useful

### Experiment 05: Proximity Fan
Things get spicy when you're close. Mario music speeds up, a fan waves faster, chaos ensues.

What I learned:
- Servo motors and PWM signals
- Variable tempo music (faster when closer = more panic)
- Multi-tasking without blocking (the holy grail)
- Making multiple outputs respond to one input
- That 300% speed Mario theme is... intense

## How I Think About Arduino

Coming from software, here's what clicked for me:

**The execution model:**
```cpp
void setup() {
  // Runs once. Like a constructor, but for the whole board.
}

void loop() {
  // Runs forever. Literally.
  // Like a game loop that never ends
  // Which it doesn't, until you unplug it
}
```

**What's different from regular coding:**
- No operating system - you're talking directly to the metal
- Timing actually matters - use `millis()` not `delay()` or everything breaks
- Not much memory - 256KB flash, 32KB RAM. That's it.
- The "interface" is physical - sensors, motors, displays
- State machines become your best friend

**The non-blocking pattern (this is important):**
```cpp
// Don't do this - blocks everything
delay(1000);

// Do this instead
unsigned long lastTime = 0;
if (millis() - lastTime > 1000) {
  // Do your thing
  lastTime = millis();
}
```

Check `MENTAL_MODEL.md` for more of my brain dumps on this.

---

## Stuff I Want to Try Next

**Quick wins:**
- RGB LED mood light that changes color based on distance
- Theremin-style musical instrument (distance = pitch)
- 360-degree obstacle detection with multiple sensors
- Data logging with Serial Plotter graphs
- Stepper motor (because precision)

**Getting fancy:**
- Temperature alarms with DHT11
- Light-following robot (photoresistor + dual servos)
- MIDI controller from sensors
- WiFi dashboard (probably need an ESP32 for that)
- Gesture tracking with multiple ultrasonic sensors

**Advanced stuff (maybe someday):**
- Mini radar display with rotating sensor
- Persistence of vision (LED strip + motor = mid-air display)
- Multi-board sensor network
- Computer vision (camera module?)
- Custom PCB design

**Practical things:**
- Plant monitor with auto-watering
- Pet feeder (because why not)
- Bike speedometer
- Door/window alerts

## Progress So Far

| Experiment | What | Hardware | Done? |
|------------|------|----------|-------|
| 01 - Blink | Digital I/O basics | Onboard LED | Yes |
| 02 - LCD Display | I2C, text | LCD 16x2 | Yes |
| 03 - Mario Game | Animation + sound | LCD, Buzzer | Yes |
| 04 - Proximity Alarm | Ultrasonic, zones | HC-SR04, Buzzer | Yes |
| 05 - Proximity Fan | Servo, variable tempo | HC-SR04, Servo, Buzzer | Yes |

**Sensors figured out:** 2/6 (Ultrasonic, LCD)
**Displays working:** 1/4 (16x2 LCD)
**Motors conquered:** 1/2 (SG90 Servo)
**Total kit explored:** About 15%

Plenty more to play with.

---

## How I Work

**Quick commands:**
```bash
# Compile
arduino-cli compile --fqbn arduino:renesas_uno:minima

# Upload to board
arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:renesas_uno:minima

# Install a library
arduino-cli lib install <LibraryName>

# Watch serial output
arduino-cli monitor -p /dev/cu.usbmodem2101 -c baudrate=9600
```

**Project structure:**
```
r4-exp/
├── experiments/
│   ├── 01-blink/
│   ├── 02-lcd-display/
│   ├── 03-mario-music-animation/
│   ├── 04-proximity-alarm/
│   └── 05-proximity-fan/
├── KIT_COMPONENTS.md       # What's in the box
├── MENTAL_MODEL.md         # How I think about this stuff
├── QUICK_REFERENCE.md      # Cheat sheet
└── README.md               # You are here
```

Each experiment has:
- The sketch (`.ino` file)
- Wiring instructions (`WIRING.txt`)
- A README explaining what I learned

## What I've Learned

**Technical stuff:**
- I2C communication (and how to find addresses when they're lost)
- Ultrasonic distance measuring (math actually works)
- PWM for servo control
- Making music with tone() functions
- Creating custom LCD characters
- Non-blocking multi-tasking (still feels like magic)
- State machines (surprisingly useful everywhere)
- Real-time sensor processing

**Hardware fundamentals:**
- Voltage levels (5V vs 3.3V - compatibility matters)
- Current limits (servos are hungry)
- Pull-up/pull-down resistors (why they exist)
- Breadboard power distribution
- Common ground (everything needs to agree on zero)
- PWM vs Digital vs Analog pins

**Problem solving:**
- Debugging I2C addresses with scanners
- Troubleshooting power issues
- Optimizing servo timing
- Writing non-blocking algorithms
- Multi-parameter adaptive systems (fancy name for "stuff that reacts")

## The Approach

> "The best way to learn is to build something that makes you smile."

Every experiment here:
- Does something real or fun
- Teaches at least one new thing
- Builds on what I already know
- Is documented so I can remember it later
- Actually works (no "in theory" nonsense)

This isn't about building the most complex thing possible. It's about understanding how things work from first principles, and having fun along the way.

## Resources

**Official docs:**
- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Arduino Uno R4 Minima Docs](https://docs.arduino.cc/hardware/uno-r4-minima/)

**Libraries I've used:**
- `LiquidCrystal_I2C` - LCD control
- `Servo` - Servo motor control
- `Wire` - I2C communication (built-in)

**Learning stuff:**
- Each experiment's README has the detailed explanations
- `MENTAL_MODEL.md` - My software-engineer-turned-hardware-person notes
- Code comments explain why, not just what

## Contributing

This is mostly my personal learning playground, but if you have ideas for cool experiments or better ways to explain stuff, I'm all ears. Open an issue or PR if you're inspired.

## License

MIT - use whatever you find here for your own learning.

## Thanks

- Arduino community for all the docs and examples
- Claude (Anthropic) for being a surprisingly good pair programmer
- All the open source library authors who made this accessible

---

**Current status:** 5 experiments done, way too many ideas brewing

**Last updated:** January 2026

---

*From console.log() to digitalWrite() - the adventure continues*
