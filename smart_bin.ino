#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Initialize the LCD (parameters: address, columns, rows)
Adafruit_LiquidCrystal lcd(0);

#define TRIG_PIN 9 // Ultrasonic sensor trigger pin
#define ECHO_PIN 10 // Ultrasonic sensor echo pin
#define FORCE_SENSOR_PIN A0 // Force sensor analog pin
#define LED_PIN 13 // LED pin

// Threshold values
const int FORCE_THRESHOLD = 300; // Adjust based on the bin's weight capacity
const int DISTANCE_THRESHOLD = 5; // Distance in cm for 'bin is full'

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize the LCD
  lcd.begin(16, 2); // 16 columns and 2 rows
  lcd.setBacklight(1); // Turn on the backlight

  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Display startup message
  lcd.print("Smart Bin System");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Measure distance from ultrasonic sensor
  int distance = measureDistance();

  // Read the force sensor value
  int forceValue = analogRead(FORCE_SENSOR_PIN);

  // Display the distance and force value on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Dist:");
  lcd.print(distance);
  lcd.print("cm   "); // Space padding to overwrite old values

  lcd.setCursor(0, 1);
  lcd.print("Force:");
  lcd.print(forceValue);
  lcd.print("    "); // Space padding to overwrite old values

  // Check if the bin is full
  if (distance <= DISTANCE_THRESHOLD || forceValue >= FORCE_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
    lcd.setCursor(0, 1);
    lcd.print("Bin is Full   "); // Overwrite old text
    Serial.println("Alert: Bin is Full!");
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off the LED
  }

  delay(1000); // Delay for 1 second before the next update
}

// Function to measure distance using the ultrasonic sensor
int measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert duration to distance in cm
  return distance;
}
