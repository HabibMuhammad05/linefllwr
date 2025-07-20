#include <Servo.h>
Servo angkat;
Servo gripper;

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
int baseSpeed = 70;
int minSpeed  = 15;
int maxSpeed  = 150;

// ----------- PID Gains ----------
float Kp = 0.4;
float Ki = 0.0005;
float Kd = 0.04;

// Anti-windup & limits
float integral = 0.0;
float integralMax = 200.0;
float integralMin = -200.0;

// PID state
int lastPosition = 0;
unsigned long lastTime = 0;

int loopDelayMs = 1; // run loop every ~10 ms

// ----------- LED + intersection setup ----------
#define LED_PIN 12
unsigned long ledTimer = 0;
bool ledActive = false;

int intersectionCount = 0;   
bool actionDone = false;     

unsigned long lastIntersectionTime = 0; // NEW cooldown timer
unsigned long intersectionCooldown = 500; // ms, adjust if needed

// ----------- Setup ----------
void setup() {
  Serial.begin(115200);

//  angkat.attach(5);   
//  gripper.attach(3);
//  
//  gripper.write(70);
//  angkat.write(135);
//  angkat.detach();   
//  gripper.detach();
  
  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  stopMotors();
  lastTime = millis();
}

// ----------- Loop ----------
void loop() {
  lineFollower();
}

// ----------- Functions ----------

// Line follower main routine
void lineFollower() {
  unsigned long now = millis();
  unsigned long dt_ms = now - lastTime;
  if (dt_ms < (unsigned long)loopDelayMs) return;
  lastTime = now;
  float dt = dt_ms / 10.0; // seconds

  readAllSensors();
  int position = getLinePosition(); 

  // --- Intersection handling ---
  if (checkIntersection(now)) {
    intersectionCount++;
    actionDone = false;  // reset action flag
  }

  // Execute action only once per intersection
  if (!actionDone) {
    switch (intersectionCount) {
      case 1: // first intersection → turn left
        Serial.println(">>> ACTION: TURN LEFT <<<");
        turnLeft();
        actionDone = true;
        break;

      case 2: // second intersection → stop
        Serial.println(">>> ACTION: STOP MOTORS <<<");
        stopMotors();
        ambilKotak();
        turnAround();
        while (1); // halt program
        break;
        
      default:
        break;
    }
  }

  // --- PID Line Following (if not in special action) ---
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
}

// ----------- Sensor & Position ----------
void readAllSensors() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(SEL_A, i & 0x01);
    digitalWrite(SEL_B, (i >> 1) & 0x01);
    delayMicroseconds(5);
    sensorValues[i]     = analogRead(X_PIN);
    sensorValues[i + 4] = analogRead(Y_PIN);
  }
}

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

// ----------- Intersection Detection ----------
bool checkIntersection(unsigned long now) {
  bool leftActive  = (sensorValues[0] > 600 || sensorValues[1] > 600);
  bool rightActive = (sensorValues[6] > 600 || sensorValues[7] > 600);
  bool centerActive = (sensorValues[3] > 600 || sensorValues[4] > 600);

  // Prevent double detection by cooldown
  if ((now - lastIntersectionTime) < intersectionCooldown) {
    return false; // ignore new detections during cooldown
  }

  if (leftActive && centerActive && rightActive) {
    Serial.println(">>> INTERSECTION DETECTED <<<");
    digitalWrite(LED_PIN, HIGH);
    ledActive = true;
    ledTimer = now;
    lastIntersectionTime = now; // reset cooldown
    return true;
  }

  if (ledActive && (now - ledTimer > 300)) {
    digitalWrite(LED_PIN, LOW);
    ledActive = false;
  }
  return false;
}

void ambilKotak(){
  delay(500);
  angkat.attach(5);   
  gripper.attach(3);
  
  angkat.write(30);     // lower gripper
  delay(1000);
  gripper.write(20);    // close claw
  delay(800);
  angkat.write(153);    // lift back up
  delay(800);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  // simple left turn (adjust timing/speed as needed)
  driveMotors(105, 0);
  delay(500);
  stopMotors();
  delay(100);
}

void turnAround() {
  // simple left turn (adjust timing/speed as needed)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 255);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 255);
  delay(230);
  stopMotors();
  delay(100);
}

// ----------- Motor control ----------
void driveMotors(int left, int right) {
  // Left forward
  if(left <= 20){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 45);
  }else{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, left);
  }

  // Right forward
  if(right <= 20){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 45);
  }else{
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, right);
  }
}
