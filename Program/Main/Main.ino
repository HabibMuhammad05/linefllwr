#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

// === SENSOR WARNA ===
Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_154MS,
  TCS34725_GAIN_4X
);

// === SENSOR TCRT5000 (4 sensor) ===
#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
int threshold = 500; // batas hitam/putih

// === MOTOR DRIVER (L298N) ===
#define ENA 10
#define IN1 6
#define IN2 7
#define ENB 11
#define IN3 8
#define IN4 9

// === SERVO ===
Servo servoLengan;
Servo servoGripper;
#define PIN_LENGAN 5
#define PIN_GRIPPER 3

// === Variabel Motor ===
int motorSpeed = 100;   // kecepatan default

// === Fungsi Motor ===
void motorKiri(int speed) {
  if (speed > 0) {
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

void maju(int speed) {
  motorKiri(speed);
  motorKanan(speed);
}

void belokKiri(int speed) {
  motorKiri(0);
  motorKanan(speed);
}

void belokKanan(int speed) {
  motorKiri(speed);
  motorKanan(0);
}

void berhenti() {
  motorKiri(0);
  motorKanan(0);
}

// === FUNGSI GRIPPER ===
void ambilBarang() {
  servoLengan.write(90); delay(1000);
  servoGripper.write(0); delay(1000);
  servoLengan.write(0); delay(1000);
  servoGripper.write(0);
  maju(motorSpeed);   // lanjut jalan lagi setelah ambil
}

// === SETUP ===
void setup() {
  Serial.begin(9600);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  servoLengan.attach(PIN_LENGAN);
  servoGripper.attach(PIN_GRIPPER);

  servoLengan.write(0);   // posisi awal lengan
  servoGripper.write(90); // posisi awal gripper (terbuka)

  if (tcs.begin()) {
    Serial.println("TCS34725 Ready");
  } else {
    Serial.println("TCS34725 Error!");
    while (1);
  }

  // === Motor langsung aktif saat start ===
  maju(motorSpeed);
}

// === LOOP ===
void loop() {
  // === LINE FOLLOWER ===
  int s1 = analogRead(S1) > threshold ? 1 : 0;
  int s2 = analogRead(S2) > threshold ? 1 : 0;
  int s3 = analogRead(S3) > threshold ? 1 : 0;
  int s4 = analogRead(S4) > threshold ? 1 : 0;

  if (s1 == 0 && s2 == 1 && s3 == 1 && s4 == 0) {
    maju(motorSpeed);
  } else if (s1 == 1 && s2 == 1 && s3 == 0 && s4 == 0 {
    belokKiri(motorSpeed);
  } else if (s1 == 0 && s2 == 0 && s3 == 1 && s4 == 1 {
    belokKanan(motorSpeed);
  } else {
    berhenti();
  }

  // === SENSOR WARNA ===
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  float red = (float)r / c * 255.0;
  float green = (float)g / c * 255.0;
  float blue = (float)b / c * 255.0;

  if (red > green && red > blue && red > 100) {
    Serial.println("Merah Terdeteksi!");
    berhenti();
    ambilBarang();
  } else if (green > red && green > blue && green > 100) {
    Serial.println("Hijau Terdeteksi!");
    berhenti();
    ambilBarang();
  } else if (blue > red && blue > green && blue > 100) {
    Serial.println("Biru Terdeteksi!");
    berhenti();
    ambilBarang();
  }
}
