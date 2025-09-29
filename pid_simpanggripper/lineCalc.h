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

void lineFollowing(){
  unsigned long now = millis();
  unsigned long dt_ms = now - lastTime;
  if (dt_ms < (unsigned long)loopDelayMs) return;
  lastTime = now;
  float dt = dt_ms / 10.0; // seconds
  displayLCD();
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
