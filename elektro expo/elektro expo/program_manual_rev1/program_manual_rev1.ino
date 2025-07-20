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

// Define push button pins
const int startButtonPin = A0;  // Start button
const int leftButtonPin = A1;   // Left turn option
const int rightButtonPin = A2;  // Right turn option
const int uTurnButtonPin = A3;  // U-turn option

// Counter for number of turns
int leftTurnCounter = 0;
int rightTurnCounter = 0;
int uTurnCounter = 0;

// Flags
bool isStartPressed = false;
bool isLeftSelected = false;
bool isRightSelected = false;
bool isUTurnSelected = false;

// Function to read sensors
int readSensor(int selectorValue1, int selectorValue2, int sensorPin) {
  digitalWrite(selectorPin1, selectorValue1);
  digitalWrite(selectorPin2, selectorValue2);
  return analogRead(sensorPin);
}

// Function to execute a left turn
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Motor kiri mundur
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);   // Motor kanan maju
  analogWrite(ENA, 150);    // Kecepatan Motor A
  analogWrite(ENB, 150);    // Kecepatan Motor B
  delay(450);               // Durasi belok kiri
}

// Function to execute a right turn
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   // Motor kiri maju
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Motor kanan mundur
  analogWrite(ENA, 150);    // Kecepatan Motor A
  analogWrite(ENB, 150);    // Kecepatan Motor B
  delay(300);               // Durasi belok kanan
}

// Function to execute a U-turn
void uTurnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   // Motor kiri maju
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Motor kanan mundur
  analogWrite(ENA, 150);    // Kecepatan Motor A
  analogWrite(ENB, 150);    // Kecepatan Motor B
  delay(500);               // Durasi putar balik kanan
}

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Set selector and motor pins as outputs
  pinMode(selectorPin1, OUTPUT);
  pinMode(selectorPin2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set push button pins as inputs
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(uTurnButtonPin, INPUT_PULLUP);

  // Initialize motors to stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  lcd.setCursor(0, 0);
  lcd.print("Set Turn Option:");
  lcd.setCursor(0, 1);
  lcd.print("L=A1 R=A2 U=A3 Start=A0");
}

void loop() {
  if (!isStartPressed) {
    if (digitalRead(leftButtonPin) == HIGH) {
      leftTurnCounter++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Left Turn: ");
      lcd.print(leftTurnCounter);
      delay(500);
    }

    if (digitalRead(rightButtonPin) == HIGH) {
      rightTurnCounter++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Right Turn: ");
      lcd.print(rightTurnCounter);
      delay(500);
    }

    if (digitalRead(uTurnButtonPin) == HIGH) {
      uTurnCounter++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("UTurn: ");
      lcd.print(uTurnCounter);
      delay(500);
    }

    if (digitalRead(startButtonPin) == HIGH) {
      isStartPressed = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Starting...");
      delay(1000);
      lcd.clear();
    }
  } else {
    // Main robot operation code
    // Deteksi persimpangan dan kontrol motor

    int sensorValues[8];
    bool sensorAboveThreshold = false;

    for (int i = 0; i < 8; i++) {
      int selectorValue1 = (i & 0b10) >> 1;
      int selectorValue2 = i & 0b01;

      if (i < 4) {
        sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin1);
      } else {
        sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin2);
      }

      if (sensorValues[i] > 200) {
        sensorAboveThreshold = true;
      }
    }

    // Check if intersection is detected
    if (sensorValues[1] > 150 && sensorValues[0] > 150 && 
        sensorValues[5] > 150 && sensorValues[6] > 150) {
      
      lcd.setCursor(0, 0);
      lcd.print("Intersection");
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      delay(500);

      if (leftTurnCounter > 0) {
        turnLeft();
        leftTurnCounter--;
      } else if (rightTurnCounter > 0) {
        turnRight();
        rightTurnCounter--;
      } else if (uTurnCounter > 0) {
        uTurnRight();
        uTurnCounter--;
      }
    } else {
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
      }

      analogWrite(ENA, speedKiri);
      analogWrite(ENB, speedKanan);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
  }
}
