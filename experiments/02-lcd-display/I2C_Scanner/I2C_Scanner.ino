/*
 * ═══════════════════════════════════════════════════════════════
 *  I2C SCANNER - Find Your LCD's Address
 * ═══════════════════════════════════════════════════════════════
 *
 *  WHAT THIS DOES:
 *  Scans all possible I2C addresses (0-127) and reports
 *  which devices are connected
 *
 *  WHY YOU NEED THIS:
 *  LCD I2C modules can have different addresses:
 *  - 0x27 (most common)
 *  - 0x3F (alternative)
 *  - Others depending on jumper configuration
 *
 *  HOW TO USE:
 *  1. Wire your LCD to Arduino (GND, VCC, SDA, SCL)
 *  2. Upload this sketch
 *  3. Open Serial Monitor (Tools → Serial Monitor)
 *  4. Set baud rate to 9600
 *  5. Note the address shown (e.g., 0x27)
 *  6. Use that address in your LCD code
 *
 * ═══════════════════════════════════════════════════════════════
 */

#include <Wire.h>

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Wait for serial port to connect
  while (!Serial);

  Serial.println("\n═══════════════════════════════════════");
  Serial.println("  I2C SCANNER - Finding Devices...");
  Serial.println("═══════════════════════════════════════\n");

  // Initialize I2C bus
  Wire.begin();

  Serial.println("Scanning I2C bus (addresses 1-126)...\n");
}

void loop() {
  byte error, address;
  int deviceCount = 0;

  // Scan all possible I2C addresses
  for(address = 1; address < 127; address++) {

    // Begin transmission to this address
    Wire.beginTransmission(address);

    // End transmission and check for errors
    error = Wire.endTransmission();

    /*
     * Error codes:
     * 0 = success (device found!)
     * 1 = data too long for buffer
     * 2 = received NACK on address (no device)
     * 3 = received NACK on data
     * 4 = other error
     */

    if (error == 0) {
      // Device found!
      Serial.print("✓ I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");  // Print leading zero for addresses < 0x10
      }
      Serial.print(address, HEX);
      Serial.println(" !");

      // Identify common devices by address
      identifyDevice(address);

      deviceCount++;
    }
    else if (error == 4) {
      Serial.print("✗ Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  // Print summary
  Serial.println("\n───────────────────────────────────────");
  if (deviceCount == 0) {
    Serial.println("No I2C devices found!");
    Serial.println("\nPossible issues:");
    Serial.println("  • Check wiring (SDA → A4, SCL → A5)");
    Serial.println("  • Ensure device has power (VCC, GND)");
    Serial.println("  • Check for loose connections");
  } else {
    Serial.print("Found ");
    Serial.print(deviceCount);
    Serial.println(" device(s)");
    Serial.println("\nUse the address shown above in your");
    Serial.println("LiquidCrystal_I2C initialization:");
    Serial.println("LiquidCrystal_I2C lcd(0xXX, 16, 2);");
  }
  Serial.println("───────────────────────────────────────\n");

  // Wait 5 seconds before scanning again
  Serial.println("Scanning again in 5 seconds...\n");
  delay(5000);
}

/*
 * Identify common I2C devices by their typical addresses
 */
void identifyDevice(byte address) {
  Serial.print("  → Likely device: ");

  switch(address) {
    case 0x27:
    case 0x3F:
      Serial.println("LCD with PCF8574 I2C backpack");
      break;
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
      Serial.println("PCF8574 I/O expander or LCD");
      break;
    case 0x68:
      Serial.println("DS1307 RTC or MPU6050 IMU");
      break;
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
      Serial.println("EEPROM (AT24C series)");
      break;
    case 0x76:
    case 0x77:
      Serial.println("BMP280/BME280 sensor");
      break;
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
      Serial.println("ADS1115 ADC or TMP102 temp sensor");
      break;
    default:
      Serial.println("Unknown device");
      break;
  }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * 📚 UNDERSTANDING I2C ADDRESSES
 * ═══════════════════════════════════════════════════════════════
 *
 * I2C addresses are 7-bit values (0-127)
 * Displayed in hexadecimal (0x00 to 0x7F)
 *
 * Common LCD addresses:
 *   0x27 = 39 in decimal (binary: 0100111)
 *   0x3F = 63 in decimal (binary: 0111111)
 *
 * Why different addresses?
 *   - Manufacturers use different default addresses
 *   - Some modules have jumpers to change address
 *   - Allows multiple devices on same I2C bus
 *
 * The I2C bus can support up to 112 devices simultaneously!
 * (Some addresses reserved for special purposes)
 *
 * ═══════════════════════════════════════════════════════════════
 */
