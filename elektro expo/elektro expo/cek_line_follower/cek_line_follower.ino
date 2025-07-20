#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2); // Sesuaikan alamat I2C jika berbeda

// Define pin connections for sensors
const int sensorPin1 = A7;
const int sensorPin2 = A6;
const int selectorPin1 = 2;
const int selectorPin2 = 13;

// Function to read sensors
int readSensor(int selectorValue1, int selectorValue2, int sensorPin) {
  // Set selector pins
  digitalWrite(selectorPin1, selectorValue1);
  digitalWrite(selectorPin2, selectorValue2);
  // Read the sensor value
  return analogRead(sensorPin);
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Set selector pins as outputs
  pinMode(selectorPin1, OUTPUT);
  pinMode(selectorPin2, OUTPUT);
}

void loop() {
  int sensorValues[8];
  bool sensorAboveThreshold = false;

  // Loop through each sensor
  for (int i = 0; i < 8; i++) {
    int selectorValue1 = (i & 0b01) > 0 ? HIGH : LOW;
    int selectorValue2 = (i & 0b10) > 0 ? HIGH : LOW;

    // Read sensor values
    if (i < 4) {
      sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin1);
    } else {
      sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin2);
    }

    // Check if any sensor value is above 400
    if (sensorValues[i] > 400) {
      sensorAboveThreshold = true;
    }
  }

  // Print sensor values on Serial Monitor for debugging
  for (int i = 0; i < 8; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print("\t"); // Tab space for better readability
  }
  Serial.println(); // Move to the next line

  // Display results on LCD based on sensor values
  if (sensorValues[0] > 400 && sensorValues[5] > 400) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
  } else if (sensorValues[1] > 400 && sensorValues[0] > 400) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 120 ");
  } else if (sensorValues[5] > 400 && sensorValues[6] > 400) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 120 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
  } else if (sensorValues[0] > 400 && sensorValues[5] <= 400) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 110 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
  } else if (sensorValues[5] > 400 && sensorValues[0] <= 400) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 110 ");
  } else if (!sensorAboveThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 80  ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 80  ");
  } else {
    lcd.clear(); // Clear LCD if no conditions are met
  }

  // Add a small delay
  delay(100);
}
