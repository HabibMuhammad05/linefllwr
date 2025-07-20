#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pin connections for sensors
const int sensorPin1 = A7;
const int sensorPin2 = A6;
const int selectorPin1 = 2;
const int selectorPin2 = 13;

// Define pin connections for motor driver
int ENA = 6;
int ENB = 3;
int IN1 = 5;
int IN2 = 4;
int IN3 = 8;
int IN4 = 7;

// Define push button pins
const int startButtonPin = A0;

// Servo declarations
Servo servo1;
Servo servo2;
int servoPin1 = 10;
int servoPin2 = 11;

// Persimpangan counter
int intersectionCounter = 0;

// Flag untuk start
bool isStartPressed = false;

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
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(280);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to execute a right turn
void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   // Motor kiri maju
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(280);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to execute a U-turn
void uTurnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   // Motor kiri maju
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(450);  // Kurangi delay untuk respon yang lebih cepat
}

void pickObject() {

    servo1.write(140);
    servo2.write(100);
    delay(1000);
    servo1.write(60);
    delay(500);
    servo2.write(25);
    delay(500);
    servo1.write(140);
}

void placeObject() {
    servo1.write(140);
    servo2.write(25);
    delay(1000);
    servo1.write(60);
    delay(1000);
    servo2.write(100);
    delay(500);
    servo1.write(140);
}


void setup() {
    Serial.begin(9600);

    servo1.attach(servoPin1);
    servo2.attach(servoPin2);

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

    // Set push button pin as input
    pinMode(startButtonPin, INPUT_PULLUP);

    // Initialize motors to stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    lcd.setCursor(0, 0);
    lcd.print("Press Start A0");
}

void loop() {
    // Menunggu tombol start ditekan
    if (!isStartPressed && digitalRead(startButtonPin) == HIGH) {
        isStartPressed = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Starting...");
        delay(1000);
        lcd.clear();
    }

    if (isStartPressed) {
        // Logika mengikuti garis
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

            if (sensorValues[i] > 150) {
                sensorAboveThreshold = true;
            }
        }

        // Cek apakah persimpangan terdeteksi
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
            delay(300);  // Kurangi delay untuk respons yang lebih cepat

            intersectionCounter++;

            switch (intersectionCounter) {
                case 1:
                    turnLeft();
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Turn Left 1");
                    break;
                case 2:
                    // Mundur selama 200ms
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Reverse 2");
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);  // Motor kiri mundur
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);  // Motor kanan mundur
                    analogWrite(ENA, 150);
                    analogWrite(ENB, 150);
                    delay(200);

                    // Mengambil objek
                    analogWrite(ENA, 0);
                    analogWrite(ENB, 0);
                    pickObject();
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("ambil objek");
                    delay(2500);
                    uTurnRight();
                    break;
                case 3:
                    turnLeft();
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Turn Left 3");
                    break;
                case 4:
                    turnRight();
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Turn Right 4");
                    break;
                case 5:
                   // Mundur selama 200ms
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Reverse 2");
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);  // Motor kiri mundur
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);  // Motor kanan mundur
                    analogWrite(ENA, 150);
                    analogWrite(ENB, 150);
                    delay(200);

                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Place Object 5");
                    analogWrite(ENA, 0);
                    analogWrite(ENB, 0);
                    placeObject();
                    uTurnRight();
                    break;
                default:
                    break;
            }
        } else {
            // Logika mengikuti garis
            int speedKiri = 80;
            int speedKanan = 80;

            // Optimasi respons kecepatan
            if (sensorValues[1] > 200 && sensorValues[0] > 200) {
                speedKiri = 70;
                speedKanan = 80;
            } else if (sensorValues[5] > 200 && sensorValues[6] > 200) {
                speedKiri = 80;
                speedKanan = 70;
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

            delay(2);  // Kurangi delay untuk respons yang lebih cepat
        }
    }
}
