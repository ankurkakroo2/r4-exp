/*
 * ═══════════════════════════════════════════════════════════════
 *  EXPERIMENT 02: LCD Display - Show Your Name
 * ═══════════════════════════════════════════════════════════════
 *
 *  WHAT THIS DOES:
 *  - Displays "Ankur" centered on the LCD screen
 *  - Shows a greeting message
 *  - Demonstrates scrolling text animation
 *  - Shows cursor positioning and control
 *
 *  HARDWARE NEEDED:
 *  - Arduino Uno R4 Minima
 *  - 16x2 LCD with I2C backpack
 *  - 4 jumper wires
 *
 *  CONNECTIONS:
 *  LCD GND  → Arduino GND
 *  LCD VCC  → Arduino 5V
 *  LCD SDA  → Arduino A4 (or SDA pin)
 *  LCD SCL  → Arduino A5 (or SCL pin)
 *
 * ═══════════════════════════════════════════════════════════════
 */

// ─────────────────────────────────────────────────────────────────
// LIBRARY INCLUDES
// ─────────────────────────────────────────────────────────────────

#include <Wire.h>                 // I2C communication library (built-in)
#include <LiquidCrystal_I2C.h>    // LCD I2C library we just installed

// ─────────────────────────────────────────────────────────────────
// LCD CONFIGURATION
// ─────────────────────────────────────────────────────────────────

/*
 * Initialize the LCD object
 *
 * LiquidCrystal_I2C lcd(address, columns, rows);
 *
 * address: I2C address of the LCD (usually 0x27 or 0x3F)
 * columns: Number of characters per row (16 for 16x2 LCD)
 * rows:    Number of rows (2 for 16x2 LCD)
 *
 * If nothing shows up, try changing 0x27 to 0x3F
 */
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Correct address from I2C scanner

// ─────────────────────────────────────────────────────────────────
// TIMING VARIABLES (For non-blocking animations)
// ─────────────────────────────────────────────────────────────────

unsigned long previousMillis = 0;    // Last time we updated display
const long interval = 3000;          // Update interval in milliseconds
int displayState = 0;                // Which display screen we're on

// ─────────────────────────────────────────────────────────────────
// SETUP FUNCTION - Runs once when Arduino powers on
// ─────────────────────────────────────────────────────────────────

void setup() {

  // ┌───────────────────────────────────────────────────────────┐
  // │ 1. Initialize Serial Monitor (for debugging)             │
  // └───────────────────────────────────────────────────────────┘

  Serial.begin(9600);              // Start serial at 9600 baud rate
  Serial.println("LCD Display Experiment Starting...");
  Serial.println("If LCD doesn't work, check:");
  Serial.println("  1. Wiring (GND, VCC, SDA, SCL)");
  Serial.println("  2. I2C address (try 0x27 or 0x3F)");
  Serial.println("  3. Contrast adjustment (blue screw on I2C module)");
  Serial.println();

  // ┌───────────────────────────────────────────────────────────┐
  // │ 2. Initialize the LCD                                     │
  // └───────────────────────────────────────────────────────────┘

  lcd.init();                      // Initialize the LCD

  /*
   * Why lcd.init()?
   * This sets up the LCD in 4-bit mode, configures the display,
   * and prepares it for use. It's like calling a constructor.
   */

  lcd.backlight();                 // Turn on the backlight

  /*
   * Why lcd.backlight()?
   * The LCD has two layers:
   * 1. Liquid crystals that form the characters (black on light background)
   * 2. LED backlight that illuminates from behind
   *
   * Without backlight, you might barely see characters (like a calculator screen)
   * With backlight, they're bright and easy to read
   *
   * You can turn it off with lcd.noBacklight()
   */

  Serial.println("LCD initialized successfully!");

  // ┌───────────────────────────────────────────────────────────┐
  // │ 3. Show welcome message                                   │
  // └───────────────────────────────────────────────────────────┘

  lcd.clear();                     // Clear any previous content

  // Display the love message
  lcd.setCursor(0, 0);            // Top row
  lcd.print("Karishma,");         // First line
  lcd.setCursor(0, 1);            // Bottom row
  lcd.print("i love you");        // Second line (with heart)

  /*
   * Why delay() here?
   * In setup(), delay() is fine because it only runs once.
   * In loop(), we'll use millis() for non-blocking timing.
   */
}

// ─────────────────────────────────────────────────────────────────
// LOOP FUNCTION - Runs forever after setup()
// ─────────────────────────────────────────────────────────────────

void loop() {
  // Message stays on screen forever! ❤️
  // Nothing to do in loop - the message was set in setup()

  /*
   * The LCD will keep displaying the message
   * "Karishma,"
   * "i love you"
   * until Arduino is powered off or reprogrammed
   */
}

// ─────────────────────────────────────────────────────────────────
// UPDATE DISPLAY FUNCTION - Handles different display screens
// ─────────────────────────────────────────────────────────────────

void updateDisplay() {

  lcd.clear();                    // Clear the screen before new content

  /*
   * State machine pattern!
   * Different screens based on displayState variable
   */

  switch(displayState) {

    // ═══════════════════════════════════════════════════════════
    // STATE 0: Show name centered with greeting
    // ═══════════════════════════════════════════════════════════
    case 0:
      Serial.println("Display State 0: Name + Greeting");

      // Calculate center position for "Ankur" (5 characters)
      // LCD has 16 columns, so center = (16 - 5) / 2 = 5.5 ≈ 5
      lcd.setCursor(5, 0);        // Row 0, column 5
      lcd.print("Ankur");

      lcd.setCursor(1, 1);        // Row 1, column 1
      lcd.print("Hello, World!");
      break;

    // ═══════════════════════════════════════════════════════════
    // STATE 1: Show with decoration
    // ═══════════════════════════════════════════════════════════
    case 1:
      Serial.println("Display State 1: Decorated");

      lcd.setCursor(0, 0);
      lcd.print("*** Ankur ***");

      lcd.setCursor(0, 1);
      lcd.print("Arduino Dev!");
      break;

    // ═══════════════════════════════════════════════════════════
    // STATE 2: Show character count demonstration
    // ═══════════════════════════════════════════════════════════
    case 2:
      Serial.println("Display State 2: Character Grid");

      // Show position markers
      lcd.setCursor(0, 0);
      lcd.print("0123456789ABCDEF");  // 16 characters - full row!

      lcd.setCursor(0, 1);
      lcd.print("16 cols x 2 rows");
      break;

    // ═══════════════════════════════════════════════════════════
    // STATE 3: Custom message
    // ═══════════════════════════════════════════════════════════
    case 3:
      Serial.println("Display State 3: Learning Message");

      lcd.setCursor(0, 0);
      lcd.print("Learning R4!");

      lcd.setCursor(0, 1);
      lcd.print("LCD I2C Ready");
      break;

    default:
      displayState = 0;           // Reset if something goes wrong
      break;
  }
}

// ─────────────────────────────────────────────────────────────────
// ADDITIONAL HELPFUL FUNCTIONS (Not used yet, but available!)
// ─────────────────────────────────────────────────────────────────

/*
 * centerText() - Helper function to center text on a row
 *
 * Usage: centerText("Hello", 0);  // Centers "Hello" on row 0
 */
void centerText(String text, int row) {
  int textLength = text.length();

  // Only center if text fits on screen
  if (textLength <= 16) {
    int position = (16 - textLength) / 2;
    lcd.setCursor(position, row);
    lcd.print(text);
  } else {
    // Text too long, just print from start
    lcd.setCursor(0, row);
    lcd.print(text.substring(0, 16));  // Truncate to 16 chars
  }
}

/*
 * scrollText() - Scroll text across the screen
 *
 * This would require more complex timing logic,
 * but here's the basic idea:
 */
void scrollText(String text, int row) {
  static int scrollPosition = 0;

  lcd.setCursor(0, row);

  // Create a sliding window of 16 characters
  String displayText = text.substring(scrollPosition, scrollPosition + 16);
  lcd.print(displayText);

  scrollPosition++;
  if (scrollPosition > text.length() - 16) {
    scrollPosition = 0;  // Loop back
  }
}

// ═══════════════════════════════════════════════════════════════
// 📚 LEARNING NOTES
// ═══════════════════════════════════════════════════════════════

/*
 * KEY CONCEPTS DEMONSTRATED:
 *
 * 1. I2C COMMUNICATION
 *    - Only 2 data wires (SDA, SCL) instead of 16!
 *    - Multiple devices can share the same I2C bus
 *    - Each device has a unique address (0x27, 0x3F, etc.)
 *
 * 2. LCD COORDINATE SYSTEM
 *    - lcd.setCursor(column, row)
 *    - Column: 0-15 (left to right)
 *    - Row: 0-1 (top to bottom)
 *    - Top-left is (0, 0)
 *
 * 3. NON-BLOCKING TIMING
 *    - Using millis() instead of delay()
 *    - Allows multitasking
 *    - Can read sensors while updating display
 *
 * 4. STATE MACHINE PATTERN
 *    - Different screens based on state variable
 *    - Clean way to handle multiple display modes
 *    - Scalable for complex UIs
 *
 * 5. LIBRARIES
 *    - Wire.h for I2C protocol
 *    - LiquidCrystal_I2C.h for LCD control
 *    - Abstraction makes complex tasks simple!
 *
 * NEXT STEPS:
 * - Add buttons to control display
 * - Show sensor data on LCD
 * - Create custom characters
 * - Build a menu system
 * - Add animations
 *
 * TROUBLESHOOTING:
 * - Nothing on screen? Adjust contrast (blue screw on I2C module)
 * - Garbage characters? Wrong I2C address (try 0x3F)
 * - Backlight on but no text? Check lcd.init() is called
 * - Text cut off? Check setCursor() positions
 */

// ═══════════════════════════════════════════════════════════════
// END OF CODE
// ═══════════════════════════════════════════════════════════════
