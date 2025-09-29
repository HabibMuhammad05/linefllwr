#include <Servo.h>
Servo angkat;
Servo gripper;

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


//  angkat.attach(5);   
//  gripper.attach(3);
//  
//  gripper.write(70);
//  angkat.write(135);
//  angkat.detach();   
//  gripper.detach();
  
