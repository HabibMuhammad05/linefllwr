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


void resetLineFollowing() {
  readAllSensors();
  int position = getLinePosition();
  integral = 0.0;
  lastPosition = position;
  lastTime = millis();

  // --- strong wake pulse ---
  driveMotors(baseSpeed + 40, baseSpeed + 40);  
  delay(200);   // short push to overcome stall
  driveMotors(baseSpeed, baseSpeed);

  Serial.println(">>> PID RESET <<<");
}

  
  // ----------- Intersection Detection ----------
bool checkIntersection(unsigned long now) {
  bool leftActive   = (sensorValues[0] > 600 || sensorValues[1] > 600);
  bool rightActive  = (sensorValues[6] > 600 || sensorValues[7] > 600);
  bool centerActive = (sensorValues[3] > 600 || sensorValues[4] > 600);

  // cooldown to prevent double trigger
  if ((now - lastIntersectionTime) < intersectionCooldown) return false;

  if (leftActive && centerActive && rightActive && !ledActive) {
    Serial.println(">>> INTERSECTION DETECTED <<<");
    lastIntersectionTime = now;
    digitalWrite(LED_PIN, HIGH);
    ledActive = true;
    return true;
  }
  
  if (ledActive && (now - lastIntersectionTime > 300)) {
    digitalWrite(LED_PIN, LOW);
    ledActive = false;
  }
  return false;
}

void handleIntersections(unsigned long now) {
  if (checkIntersection(now)) {
    intersectionCount++;
    actionDone = false;
  }

  if (!actionDone) {
    switch (planNum){
      case 0: // dari start ke finish langsung
        switch (intersectionCount) {
          case 1: actionDone = true; break;
          case 2: turnLeft();        actionDone = true;  break;
          case 3: actionDone = true; break;
          case 4: actionDone = true;  break;
          case 5: actionDone = true;  break;
          case 6: actionDone = true;  break;
          case 7: maju();             stopMotors();      while(1);       break;
          default: break;
        }
      break;
      case 1: // jika warna kotak dan gudang sejajar
        switch (intersectionCount) {
          case 1: turnLeft();        actionDone = true;  break;
          case 2: stopMotors();      ambilKotak();       turnAround();    resetLineFollowing();   actionDone = true;   break;
          case 3: actionDone = true; break;
          case 4: stopMotors();      lepasKotak();       turnAround();    actionDone = true;    break;
          case 5: turnRight();       actionDone = true;  break;
          case 6: maju();            while(1);           break;
          default: break;
        }
      break;
      case 2: // jika warna kotak diatas 1
        switch (intersectionCount) {
          case 1: turnLeft();        actionDone = true;  break;
          case 2: stopMotors();      ambilKotak();       turnAround();    resetLineFollowing();   actionDone = true;   break;
          case 3: turnLeft();        actionDone = true;  break;
          case 4: turnRight();       actionDone = true;  break;
          case 5: stopMotors();      lepasKotak();       turnAround();    actionDone = true;    break;
          case 6: turnLeft();        actionDone = true;  break;
          case 7: maju();            while(1);  break;
          default: break;
        }
      break;
      case 3: // jika warna kotak diatas 2
        switch (intersectionCount) {
          case 1: turnLeft();        actionDone = true;  break;
          case 2: stopMotors();      ambilKotak();       turnAround();    resetLineFollowing();   actionDone = true;   break;
          case 3: turnLeft();        actionDone = true;  break;
          case 4: actionDone = true; break;
          case 5: turnRight();       actionDone = true;  break;
          case 6: stopMotors();      lepasKotak();       turnAround();    actionDone = true;    break;
          case 7: turnLeft();        actionDone = true;  break;
          case 8: maju();            while(1);  break;
          default: break;
        }
      break;
      case 4: // jika warna kotak dibawah 1
        switch (intersectionCount) {
          case 1: turnLeft();        actionDone = true;  break;
          case 2: stopMotors();      ambilKotak();       turnAround();    resetLineFollowing();   actionDone = true;   break;
          case 3: turnRight();       actionDone = true; break;
          case 4: turnLeft();        actionDone = true;    break;
          case 5: stopMotors();      lepasKotak();       turnAround();    actionDone = true;    break;
          case 6: turnRight();       actionDone = true;    break;
          case 7: maju();            while(1);  break;
          default: break;
        }
      break;
      case 5: // jika warna kotak dibawah 2
        switch (intersectionCount) {
          case 1: turnLeft();        actionDone = true;  break;
          case 2: stopMotors();      ambilKotak();       turnAround();    resetLineFollowing();   actionDone = true;   break;
          case 3: turnRight();       actionDone = true; break;
          case 4: actionDone = true; break;
          case 5: turnLeft();        actionDone = true;    break;
          case 6: stopMotors();      lepasKotak();       turnAround();    actionDone = true;    break;
          case 7: turnRight();       actionDone = true;    break;
          case 8: maju();            while(1);  break;
          default: break;
        }
      break;
      case 6: // kejar retry 1
        switch (intersectionCount) {
          case 1: actionDone = true; break;
          case 2: turnLeft();        actionDone = true;  break;
          case 3: actionDone = true; break;
          case 4: maju();            stopMotors();    while(1);  break;
          default: break;
        }
      break;
      case 7: // loncat 1 retry
        switch (intersectionCount) {
          case 1: actionDone = true; break;
          case 2: turnLeft();        actionDone = true;  break;
          case 3: actionDone = true; break;
          case 4: actionDone = true; break;
          case 5: maju();            stopMotors();    while(1);  break;
          default: break;
        }
      break;
      case 8: // loncat 2 retry
        switch (intersectionCount) {
          case 1: actionDone = true; break;
          case 2: actionDone = true; break;
          case 3: maju();            stopMotors();    while(1);  break;
          default: break;
        }
      break;
    }
  }
  
}





void lineFollowing() {
  unsigned long now = millis();
  unsigned long dt_ms = now - lastTime;

  // run-timing guard (same as before)
  if (dt_ms < (unsigned long)loopDelayMs) return;

  // --- DETECT LONG PAUSE (resume from blocking action) ---
  const unsigned long MAX_DT_MS = 250; // if longer than this, treat as "resumed"
  if (dt_ms > MAX_DT_MS) {
    // Re-sample sensors and reset PID state to avoid huge integral/derivative spikes
    readAllSensors();
    int position = getLinePosition();
    integral = 0.0;                // clear integral windup
    lastPosition = position;       // avoid derivative spike
    lastTime = now;                // restart timing
    // Optionally send a gentle "wake" command to motors so they can start smoothly
    driveMotors(baseSpeed, baseSpeed);
    // Debug - resume event
    Serial.print("RESUME from action, dt_ms="); Serial.print(dt_ms);
    return; // skip a full PID cycle this loop — next call will be normal
  }

  // Normal small-dt operation
  lastTime = now;
  float dt = dt_ms / 10.0;

  readAllSensors();
  int position = getLinePosition();

  // Debug: small periodic print (remove or throttle if noisy)
  if (millis() % 200 < 10) {
    Serial.print("========dt_ms: "); Serial.print(dt_ms);
    Serial.print(" pos: "); Serial.print(position);
    Serial.print(" I: "); Serial.print(integral);
  }

  // PID
  float error = (float)position;
  integral += error * dt;
  // anti-windup clamp (you already have these)
  if (integral > integralMax) integral = integralMax;
  if (integral < integralMin) integral = integralMin;

  float derivative = 0.0;
  if (dt > 0) derivative = (error - (float)lastPosition) / dt;

  float correction = Kp * error + Ki * integral + Kd * derivative;
  lastPosition = position;

  int leftSpeed  = constrain((int)round(baseSpeed - correction), minSpeed, maxSpeed);
  int rightSpeed = constrain((int)round(baseSpeed + correction), minSpeed, maxSpeed);

  // Debug: occasionally print motor outputs
  if (millis() % 200 < 10) {
    Serial.print("=======L: "); Serial.print(leftSpeed);
    Serial.print(" R: "); Serial.println(rightSpeed);
  }
  driveMotors(leftSpeed, rightSpeed);
}
