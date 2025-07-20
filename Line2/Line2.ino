#include <Wire.h>

// === MULTIPLEXER UNTUK SENSOR TCRT5000 (8 sensor) ===
#define SEL_A A0
#define SEL_B A1
#define X_PIN A2   // channel multiplexer 1
#define Y_PIN A3   // channel multiplexer 2

#define PB 2

int sensorValues[8];

int sensorOrder[8] = {5, 7, 6, 4, 2, 1, 0, 3};    

int sensorposition[8] = {-3500, -2500, -1500, -500, 500, 1500, 2500, 3500}; 

// === MOTOR DRIVER (L298N) ===
#define ENA 10
#define IN1 6
#define IN2 7
#define ENB 11
#define IN3 8
#define IN4 9

const long interval = 1000;     
unsigned long previousMillis = 0;

int basespeed = 10;     
float Kp = 30, Ki = 0.9, Kd = 5.4;
long lastError = 0, integral = 0;

void motorKiri(int speed){
  if(speed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (speed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  analogWrite(ENA, abs(speed));
}

void motorKanan(int speed) {
  if (speed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (speed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  analogWrite(ENB, abs(speed));
}

void berhenti() {
  motorKiri(0);
  motorKanan(0);
}

void readAllSensor(){
  for (int i = 0; i < 4; i++){
    digitalWrite(SEL_A, i & 0x01);          
    digitalWrite(SEL_B,(i >> 1) & 0x01);
    delayMicroseconds(5);

    sensorValues[i] = analogRead(X_PIN);     
    sensorValues[i + 4] = analogRead(Y_PIN);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(PB, INPUT_PULLUP);
}

void loop() {
  readAllSensor();         

  long weightedSum = 0;
  long total = 0;

  // === Hitung PID berdasarkan sensor ===
  for (int i = 0; i < 8; i++) {
    int idx = sensorOrder[i];        
    int value = 1023 - sensorValues[idx];  

    if (value < 0) value = 0;        

    weightedSum += (long)value * sensorposition[i];
    total += value;
  }

  long position = (total == 0) ? lastError : weightedSum / total;

  long error = position;
  integral += error;
  long derivative = error - lastError;

  long correction = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

  int leftMotorSpeed  = basespeed - correction;
  int rightMotorSpeed = basespeed + correction;

  leftMotorSpeed  = constrain(leftMotorSpeed, -40, 40);
  rightMotorSpeed = constrain(rightMotorSpeed, -40, 40);

  motorKiri(leftMotorSpeed);
  motorKanan(rightMotorSpeed);

  // === Cetak data sensor setiap interval ===
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // tampilkan semua sensor (urut kiri ke kanan)
    for (int i = 0; i < 8; i++) {
      int idx = sensorOrder[i];
      int value = 1023 - sensorValues[idx];  
      if (value < 0) value = 0;

      Serial.print("S");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(value);     // nilai setelah dibalik
      Serial.print(" (raw:");
      Serial.print(sensorValues[idx]); // nilai asli
      Serial.print(")\t");
    }

    // tampilkan info PID & motor
    Serial.print(" | L: ");
    Serial.print(leftMotorSpeed);
    Serial.print(" R: ");
    Serial.print(rightMotorSpeed);
    Serial.print(" Corr: ");
    Serial.println(correction);
  }
}
