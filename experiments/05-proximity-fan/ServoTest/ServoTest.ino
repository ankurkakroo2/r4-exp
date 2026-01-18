/*
 * SERVO POWER TEST
 * Tests if servo has enough power to complete full swing
 *
 * WIRING (MINIMAL):
 * Servo Brown  → GND
 * Servo Red    → 5V
 * Servo Orange → Pin 11
 *
 * DISCONNECT LCD temporarily for this test!
 */

#include <Servo.h>

Servo testServo;

void setup() {
  Serial.begin(9600);
  Serial.println("=== SERVO POWER TEST ===");
  Serial.println("Testing full 0-180° sweep...");

  testServo.attach(11);
  delay(1000);
}

void loop() {
  // Test 1: Slow full sweep
  Serial.println("\n--- Test 1: Slow Sweep (1 sec delay) ---");

  Serial.println("Moving to 0°...");
  testServo.write(0);
  delay(1000);

  Serial.println("Moving to 90°...");
  testServo.write(90);
  delay(1000);

  Serial.println("Moving to 180°...");
  testServo.write(180);
  delay(1000);

  Serial.println("Back to 90°...");
  testServo.write(90);
  delay(2000);

  // Test 2: Fast oscillation (like fan)
  Serial.println("\n--- Test 2: Fast Oscillation (200ms) ---");
  for(int i = 0; i < 10; i++) {
    Serial.print("Swing #"); Serial.println(i+1);
    testServo.write(0);
    delay(200);
    testServo.write(180);
    delay(200);
  }

  Serial.println("\n--- Tests Complete ---");
  Serial.println("Repeating in 3 seconds...\n");
  delay(3000);
}
