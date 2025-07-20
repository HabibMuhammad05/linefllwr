
// ----------- Motor Driver Setup ----------
#define ENA 10   // PWM Left Motor
#define IN1 6
#define IN2 7
#define ENB 11   // PWM Right Motor
#define IN3 8
#define IN4 9

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  driveMotors(70, 70);
  delay(2000);
  stopMotors();
  delay(1000);
  driveMotors(150, 20);
  delay(2000);
  stopMotors();
  delay(1000);
  driveMotors(20, 150);
  delay(2000);
  stopMotors();
  delay(1000);
  driveMotors(-150, -150);
  delay(2000);
  stopMotors();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// ----------- Motor control ----------
void driveMotors(int left, int right) {
  if(left > 0){
    if(left <= 20){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 45);
    }else{
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, left);
    }
  }else{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, -left);
  }
  if (right > 0){
    if(right <= 20){
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 45);
    }else{
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, right);
    }
  }else{
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 45);
  }
  
//  if(left <= 20 && left >= 0){
//    digitalWrite(IN1, HIGH);
//    digitalWrite(IN2, LOW);
//    analogWrite(ENA, 45);
//  }else if( left < 0){
//    digitalWrite(IN1, HIGH);
//    digitalWrite(IN2, LOW);
//    analogWrite(ENA, left);
//  }else{
//    digitalWrite(IN1, LOW);
//    digitalWrite(IN2, HIGH);
//    analogWrite(ENA, left);
//  }
//
//  if(right <= 20 && right >= 0){
//    digitalWrite(IN3, HIGH);
//    digitalWrite(IN4, LOW);
//    analogWrite(ENB, 45);
//  }else if(right < 0){
//    digitalWrite(IN3, HIGH);
//    digitalWrite(IN4, LOW);
//    analogWrite(ENB, right);
//  }else{
//    digitalWrite(IN3, LOW);
//    digitalWrite(IN4, HIGH);
//    analogWrite(ENB, right);
//  }
}


void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
