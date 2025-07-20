//----------- Servo Setup ----------
#include <Servo.h>
Servo angkat;
Servo gripper;
bool ServoRun = true;
// ----------- MUX & Sensor Setup ----------
#define SEL_A A0
#define SEL_B A1
#define X_PIN A2
#define Y_PIN A3

int sensorValues[8];
int sensorOrder[8] = {5, 7, 6, 4, 2, 1, 0, 3}; // Your given order

// ----------- Motor Driver Setup ----------
#define ENA 10   // PWM Left Motor
#define IN1 6
#define IN2 7
#define ENB 11   // PWM Right Motor
#define IN3 8
#define IN4 9

// ----------- Speed Settings ----------
int baseSpeed = 70;   // center speed
int minSpeed  = 15;   // min PWM to avoid stall
int maxSpeed  = 155;  // allow a bit more room for correction

// ----------- PID Gains ----------
float Kp = 0.5;     
float Ki = 0.0005;  
float Kd = 0.02;    

// Anti-windup & limits
float integral = 0.0;
float integralMax = 200.0;
float integralMin = -200.0;

// PID state
int lastPosition = 0;
unsigned long lastTime = 0;

int loopDelayMs = 1;

// ----------- Stop Sign Detector ----------
bool stopDetected = false;
int darkThreshold = 500;    // adjust based on sensor values (lower = darker)
int darkCountMin = 6;       // how many sensors must see "dark" to confirm stop

// ----------- Setup ----------
void setup() {
  Serial.begin(115200);
  
//  angkat.attach(5);   
//  gripper.attach(3);
//  
//  gripper.write(70);
//  angkat.write(135);
//      angkat.detach();   
//      gripper.detach();

  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
  lastTime = millis();
}

// ----------- Loop ----------
void loop() {
  if (stopDetected){ 
    if(ServoRun){
      ServoRun = false;
      
      delay(1500);
      angkat.attach(5);   
      gripper.attach(3);
      angkat.write(46);     // lower gripper
      delay(1000);
      gripper.write(20);    // close claw
      delay(800);
      angkat.write(135);    // lift back up
      delay(800);
      Serial.println("Gripper sequence complete!");
//      angkat.detach();   
//      gripper.detach();
    }
    return; // once stopped, stay stopped
  }

  unsigned long now = millis();
  unsigned long dt_ms = now - lastTime;
  if (dt_ms < (unsigned long)loopDelayMs) return;
  lastTime = now;
  float dt = dt_ms / 10.0; // seconds

  readAllSensors();

  // --- STOP SIGN CHECK ---
  if (checkStopSign()) {
    stopMotors();
    stopDetected = true;
    Serial.println("STOP SIGN DETECTED!");
    return;
  }

  int position = getLinePosition(); // negative left, positive right

  // --- PID ---
  float error = (float)position;
  integral += error * dt;
  if (integral > integralMax) integral = integralMax;
  if (integral < integralMin) integral = integralMin;

  float derivative = 0.0;
  if (dt > 0) derivative = (error - (float)lastPosition) / dt;

  float correction = Kp * error + Ki * integral + Kd * derivative;

  lastPosition = position;

  int leftSpeed  = (int)round(baseSpeed - correction);
  int rightSpeed = (int)round(baseSpeed + correction);

  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  driveMotors(leftSpeed, rightSpeed);

  // Debug
  Serial.print("pos: "); Serial.print(position);
  Serial.print(" | corr: "); Serial.print(correction, 2);
  Serial.print(" | L: "); Serial.print(leftSpeed);
  Serial.print(" | R: "); Serial.print(rightSpeed);
  Serial.print(" | I: "); Serial.print(integral, 2);
  Serial.print(" | D: "); Serial.println(derivative, 2);
}

// ----------- Functions ----------

// Read all 8 sensors using MUX
void readAllSensors() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(SEL_A, i & 0x01);
    digitalWrite(SEL_B, (i >> 1) & 0x01);
    delayMicroseconds(5);
    sensorValues[i]     = analogRead(X_PIN);
    sensorValues[i + 4] = analogRead(Y_PIN);
  }
}

// Weighted average position: 0..7000 -> center 3500 -> subtract to make center 0
int getLinePosition() {
  long sum = 0;
  long total = 0;
  for (int i = 0; i < 8; i++) {
    int idx = sensorOrder[i];
    int value = sensorValues[idx];
    sum += (long)value * (i * 1000);
    total += value;
  }
  if (total == 0) return 0;
  return (int)(sum / total - 3500);
}

// Detect stop line (horizontal dark line across all sensors)
bool checkStopSign() {
  int darkCount = 0;
  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] > darkThreshold) {
      darkCount++;
    }
  }
  return (darkCount >= darkCountMin);
}

// Motor control (forward only)
void driveMotors(int left, int right) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, left);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, right);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
