int ENA = 6;   // Pin PWM untuk Motor A (kiri)
int ENB = 3;   // Pin PWM untuk Motor B (kanan)
int IN1 = 5;   // Motor kiri maju
int IN2 = 4;   // Motor kiri mundur
int IN3 = 8;   // Motor kanan maju
int IN4 = 7;   // Motor kanan mundur

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

//   // Mengatur arah Motor A
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);

//   // Mengatur arah Motor B
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
  }

void loop() {
    // Mengatur arah Motor A
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);

  // Mengatur arah Motor B
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  analogWrite(ENA, 80);  // Mengatur kecepatan Motor A (0-255)
  analogWrite(ENB, 80);  // Mengatur kecepatan Motor B (0-255)
  delay(1000);
    // digitalWrite(IN1, HIGH);
    // digitalWrite(IN2, LOW);   // Motor kiri maju
    // digitalWrite(IN3, LOW);
    // digitalWrite(IN4, HIGH);  // Motor kanan mundur
    // analogWrite(ENA, 180);
    // analogWrite(ENB, 170);
    // delay(1000);  // Kurangi delay untuk respon yang lebih cepat


}
