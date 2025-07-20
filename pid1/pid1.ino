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

// ----------- Speed Settings (your working window) ----------
int baseSpeed = 70;   // center speed (45–80 worked for you)
int minSpeed  = 15;   // min PWM to avoid stall
int maxSpeed  = 150;  // allow a bit more room for correction

// ----------- PID Gains (start here) ----------
float Kp = 0.4;     // = 1 / divisor(10)
float Ki = 0.0005;  // small integral to fix bias (start very small or 0)
float Kd = 0.04;    // derivative to make it react faster

// Anti-windup & limits
float integral = 0.0;
float integralMax = 200.0;  // clamp integral to prevent windup
float integralMin = -200.0;

// PID state
int lastPosition = 0;
unsigned long lastTime = 0;

int loopDelayMs = 1; // run loop every ~10 ms (adjustable)

// ----------- Setup ----------
void setup() {
  Serial.begin(115200);

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
  unsigned long now = millis();
  unsigned long dt_ms = now - lastTime;
  if (dt_ms < (unsigned long)loopDelayMs) return; // simple timing
  lastTime = now;
  float dt = dt_ms / 10.0; // seconds

  readAllSensors();
  int position = getLinePosition(); // negative left, positive right

  // --- PID on position (position is large, gains tuned accordingly) ---
  float error = (float)position; // use raw position as "error"
  // Integral
  integral += error * dt * 1.0; // multiply by 1.0 to keep units clear
  // anti-windup clamp
  if (integral > integralMax) integral = integralMax;
  if (integral < integralMin) integral = integralMin;

  // Derivative
  float derivative = 0.0;
  if (dt > 0) derivative = (error - (float)lastPosition) / dt;

  float correction = Kp * error + Ki * integral + Kd * derivative;

  lastPosition = position;

  // Convert correction to PWM offset. Because error can be large, correction may be large; we'll apply and then clamp.
  int leftSpeed  = (int)round(baseSpeed - correction);
  int rightSpeed = (int)round(baseSpeed + correction);

  // Ensure minimum/maximum and avoid stalling
  leftSpeed  = constrain(leftSpeed,  minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  driveMotors(leftSpeed, rightSpeed);

  // Debug: print important values (use Serial Monitor at 115200)
  Serial.print("pos: "); Serial.print(position);
  Serial.print(" | err: "); Serial.print(error, 1);
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
    delayMicroseconds(5); // let mux settle (small)
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
    sum += (long)value * (i * 1000); // position weight 0,1000,...7000
    total += value;
  }
  if (total == 0) return 0;
  return (int)(sum / total - 3500);
}

// Motor control (forward only)
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

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
