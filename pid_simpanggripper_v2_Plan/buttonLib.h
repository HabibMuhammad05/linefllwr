#define BTN1 4    // Digital pin
#define BTN2 A6   // Analog-only pin
#define BTN3 A7   // Analog-only pin

#define DEBOUNCE 50
#define THRESHOLD 512   // threshold for A6/A7

// button states
bool lastState[3] = {HIGH, HIGH, HIGH};
unsigned long lastChange[3] = {0, 0, 0};
bool btnToggle[3] = {false, false, false};  // one toggle var per button
bool motorTrigger = false;  // will be set true on press, cleared after running



void checkButton(uint8_t index, int pin, bool isAnalog) {
  bool reading;

  if (isAnalog) {
    reading = (analogRead(pin) < THRESHOLD) ? LOW : HIGH;
  } else {
    reading = digitalRead(pin);
  }

  if (reading != lastState[index]) {
    if (millis() - lastChange[index] > DEBOUNCE) {
      lastChange[index] = millis();
      lastState[index] = reading;

      if (reading == LOW) {
        if (index == 1) {
          // Button 2 (A6) → cycle planNum 0–3
          planNum = (planNum + 1) % 9;
          Serial.print("planNum = ");
          Serial.println(planNum);

        } else if (index == 0) {
          // Button 1 (D4) → toggle as before
          btnToggle[index] = !btnToggle[index];
        
          Serial.print("Button ");
          Serial.print(index + 1);
          Serial.print(" TOGGLE -> ");
          Serial.println(btnToggle[index] ? "ON" : "OFF");
        
          // also trigger the motor sequence once
          motorTrigger = true;
        }
         else {
          // Button 3 (A7) → toggle
          planNum = (planNum - 1) % 9;
          Serial.print("planNum = ");
          Serial.println(planNum);
        }
      }
    }
  }
}



void setupButton(){
  pinMode(BTN1, INPUT_PULLUP);
}

void readButtons() {
  checkButton(0, BTN1, false); // digital
  checkButton(1, BTN2, true);  // analog
  checkButton(2, BTN3, true);  // analog
}
