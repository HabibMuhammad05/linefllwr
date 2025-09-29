#include <Servo.h>
Servo angkat;
Servo gripper;

void servoSetup(){
  angkat.attach(10);   
  gripper.attach(3);
}

void ambilKotak(){
  delay(500);
  servoSetup();
  gripper.write(110);
  delay(300);
  for (uint8_t pos = 135; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
    angkat.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  delay(400);
  gripper.write(0);    // close claw
  delay(800);
  for (uint8_t pos = 30; pos <= 135; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    angkat.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  delay(300);
}
void lepasKotak(){
  delay(500);
  servoSetup();
  for (uint8_t pos = 135; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
    angkat.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  gripper.write(110);
  delay(300);
  for (uint8_t pos = 30; pos <= 135; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    angkat.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  gripper.write(10);    // close claw
  delay(300);
//  angkat.detach();   
//  gripper.detach();
//  delay(300);
}


//  angkat.attach(5);   
//  gripper.attach(3);
//  
//  gripper.write(70);
//  angkat.write(135);
//  angkat.detach();   
//  gripper.detach();
  
