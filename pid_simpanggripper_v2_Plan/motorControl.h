void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ----------- Motor control ----------
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

void turnLeft() {
  driveMotors(130, 0);
  delay(500);
  stopMotors();
  delay(100);
}
void turnRight() {
  driveMotors(0, 130);
  delay(500);
  stopMotors();
  delay(100);
}

void mundur()
{
  driveMotors(255, 255);
  delay(1000);
  stopMotors();
  delay(100);
}
void maju(){
  driveMotors(255, 130);
  delay(250);
  stopMotors();
  delay(100);
}

void turnAround() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 245);
  delay(350);
  stopMotors();
  delay(100);
}
