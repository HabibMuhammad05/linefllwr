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

  // Check if the intersection is detected (sensors 1, 0, 5, 6 above 100)
  if (sensorValues[1] > 100 && sensorValues[0] > 100 && 
      sensorValues[5] > 100 && sensorValues[6] > 100 ) {
    // Stop the robot
    lcd.setCursor(0, 0);
    lcd.print("Intersection  ");
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(2000); // Stop for 2 seconds
  } else {
    // Kodingan mempertahankan di tengah garis, dapat dioptimasi seperti ini:
     // Code to follow line
      int speedKiri = 80;
      int speedKanan = 80;
      if (sensorValues[1] > 200 && sensorValues[0] > 200) {
        speedKiri = 75;
        speedKanan = 80;
      } else if (sensorValues[5] > 200 && sensorValues[6] > 200) {
        speedKiri = 80;
        speedKanan = 75;
      } else if (sensorValues[1] > 200) {
        speedKiri = 50;
        speedKanan = 80;
      } else if (sensorValues[3] > 200) {
        speedKiri = 50;
        speedKanan = 100;
      } else if (sensorValues[2] > 200) {
        speedKiri = 0;
        speedKanan = 150;
      } else if (sensorValues[0] > 200) {
        speedKiri = 80;
        speedKanan = 80;
      } else if (sensorValues[5] > 200) {
        speedKiri = 80;
        speedKanan = 80;
      } else if (sensorValues[6] > 200) {
        speedKiri = 80;
        speedKanan = 50;
      } else if (sensorValues[4] > 200) {
        speedKiri = 100;
        speedKanan = 50;
      } else if (sensorValues[7] > 200) {
        speedKiri = 150;
        speedKanan = 0;
      } else if (!sensorAboveThreshold) {
        speedKiri = 0;
        speedKanan = 0;
      } else {
        speedKiri = 80;
        speedKanan = 80;
      }

      analogWrite(ENA, speedKiri);
      analogWrite(ENB, speedKanan);

        // Set motor directions (both forward)
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
      
      // Small delay for stability
      delay(5);
  }
}
