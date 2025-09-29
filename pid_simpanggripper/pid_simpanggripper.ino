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

  if(btnToggle[0] == HIGH){
    lineFollowing();
  }
}
