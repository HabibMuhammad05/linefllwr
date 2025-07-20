#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2); // Sesuaikan alamat I2C jika berbeda

// Define pin connections for sensors
const int sensorPin1 = A7;
const int sensorPin2 = A6;
const int selectorPin1 = 2;
const int selectorPin2 = 3;

// Define pin connections for motor driver
int ENA = 6;   // Pin PWM untuk Motor A (kiri)
int ENB = 9;   // Pin PWM untuk Motor B (kanan)
int IN1 = 5;   // Motor kiri maju
int IN2 = 4;   // Motor kiri mundur
int IN3 = 8;   // Motor kanan maju
int IN4 = 7;   // Motor kanan mundur

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

  // Set motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize motors to stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  int sensorValues[8];
  bool sensorAboveThreshold = false;

  // Loop through each sensor
  for (int i = 0; i < 8; i++) {
    int selectorValue1 = (i & 0b10) >> 1; // Extract the second bit
    int selectorValue2 = i & 0b01;        // Extract the first bit

    // Read sensor values
    if (i < 4) {
      sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin1);
    } else {
      sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin2);
    }

    // Check if any sensor value is above 200
    if (sensorValues[i] > 200) {
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

  // Initialize motor speed variables
  int speedKiri = 0;
  int speedKanan = 0;

  // Display results on LCD and set motor speeds based on sensor values
  if (sensorValues[0] > 200 && sensorValues[5] > 200) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
    speedKiri = 100;
    speedKanan = 100;
  } else if (sensorValues[1] > 200 && sensorValues[0] > 200) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 75 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 80 ");
    speedKiri = 75;
    speedKanan = 80;
  } else if (sensorValues[1] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 50 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 90 ");
    speedKiri = 50;
    speedKanan = 90;
  } else if (sensorValues[3] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 50 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 95 ");
    speedKiri = 50;
    speedKanan = 95;
  } else if (sensorValues[3] > 200 && sensorValues[1] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 50 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
    speedKiri = 50;
    speedKanan = 100;
  } else if (sensorValues[6] > 200 && sensorValues[5] > 200) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 80 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 75 ");
    speedKiri = 80;
    speedKanan = 75;
  } else if (sensorValues[0] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
    speedKiri = 100;
    speedKanan = 100;
  } else if (sensorValues[5] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
    speedKiri = 100;
    speedKanan = 100;
  } else if (sensorValues[6] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 90 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 50 ");
    speedKiri = 90;
    speedKanan = 50;
  } else if (sensorValues[2] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 0 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 100 ");
    speedKiri = 0;
    speedKanan = 150;
  } else if (sensorValues[4] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 95 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 50 ");
    speedKiri = 95;
    speedKanan = 50;
  } else if (sensorValues[6] > 200 && sensorValues[4] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 100 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 50 ");
    speedKiri = 100;
    speedKanan = 50;
  } else if (sensorValues[7] > 200 && sensorValues[4] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 130 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 50 ");
    speedKiri = 130;
    speedKanan = 50;
  } else if (sensorValues[7] > 200 ) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 150 ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 0 ");
    speedKiri = 150;
    speedKanan = 0;
  } else if (!sensorAboveThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("kiri : 0  ");
    lcd.setCursor(0, 1);
    lcd.print("kanan: 0  ");
    speedKiri = 0;
    speedKanan = 0;
  } else {
    lcd.clear(); // Clear LCD if no conditions are met
  }
  
  // Set motor speeds
  analogWrite(ENA, speedKiri);
  analogWrite(ENB, speedKanan);

  // Set motor directions (both forward)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // Add a small delay
  delay(10);
}
