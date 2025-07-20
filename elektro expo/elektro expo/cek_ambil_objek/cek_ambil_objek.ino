#include <Servo.h>

// Define pin connections for motor driver
int ENA = 6;
int ENB = 9;
int IN1 = 5;
int IN2 = 4;
int IN3 = 8;
int IN4 = 7;

// Define servo objects
Servo servo1; // Servo untuk gerakan naik turun
Servo servo2; // Servo untuk gerakan menjepit

// Define pin connections for servos
int servo1Pin = 10;  // Sesuaikan dengan pin yang Anda gunakan untuk servo 1
int servo2Pin = 11;  // Sesuaikan dengan pin yang Anda gunakan untuk servo 2

void setup() {
  // Attach servos to their respective pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);


    // Set selector and motor pins as outputs

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);


}

void loop() {


pickObject();
delay(4000);

turnLeft();
delay(2000);

placeObject();
delay(4000);

turnRight();
delay(2000);

}



// Function to execute a left turn
void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);  
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);   
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(300);  
}

// Function to execute a right turn
void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(300);  
}

// Function to execute a U-turn
void uTurnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(450);  
} 



// Function to pick up an object
void pickObject() {
    servo1.write(140);
    servo2.write(100);  // Gerakan 1
    delay(1000);
    servo1.write(60);
    delay(500);         // Gerakan 2
    servo2.write(25);
    delay(500);         // Gerakan 3
    servo1.write(140);  // Gerakan 4
}

// Function to place an object
void placeObject() {
    servo1.write(140);
    servo2.write(25);
    delay(1000);        // gerakan 5
    servo1.write(60);   // Gerakan 6
    delay(1000);
    servo2.write(100);  // Gerakan 7
    delay(500);
    servo1.write(140);  // Gerakan 8

}