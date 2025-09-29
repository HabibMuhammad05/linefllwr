#include "globalVars.h"
#include "servoLib.h"
#include "lcdLib.h"    
#include "motorControl.h"   
#include "lineCalc.h"  
#include "buttonLib.h"         

void setup() {
  Serial.begin(115200);                 
  lcdSetup();
  ioSetup();
  setupButton();
  stopMotors();
  lastTime = millis();
}

void loop() {
  displayLCD();
  readButtons();
  unsigned long now = millis();
  
  if (btnToggle[0] == HIGH) {
      handleIntersections(now);
      lineFollowing();
    }
  
    // one-shot motor sequence
    if (motorTrigger) {
      driveMotors(150, 170);
      delay(80);        // (can be replaced with millis later)
      stopMotors();
      motorTrigger = false;  // clear flag so it runs only once
    }
}
