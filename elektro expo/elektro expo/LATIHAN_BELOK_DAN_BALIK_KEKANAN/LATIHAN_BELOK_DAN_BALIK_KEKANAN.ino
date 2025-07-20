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
}

void loop() {
  // Belok kiri
  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, HIGH);  // Motor kiri mundur
  // digitalWrite(IN3, HIGH);
  // digitalWrite(IN4, LOW);   // Motor kanan maju
  // analogWrite(ENA, 150);   // Mengatur kecepatan Motor A (PWM = 150)
  // analogWrite(ENB, 150);   // Mengatur kecepatan Motor B (PWM = 150)
  // delay(300);  // Belok kiri selama 1 detik

  // // berhenti
  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, LOW);   
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, LOW);  
  // analogWrite(ENA, 0);   // Mengatur kecepatan Motor A (PWM = 150)
  // analogWrite(ENB, 0);   // Mengatur kecepatan Motor B (PWM = 150)
  // delay(4000);  // 


  // // Belok kanan
  // digitalWrite(IN1, HIGH);
  // digitalWrite(IN2, LOW);   // Motor kiri maju
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);  // Motor kanan mundur
  // analogWrite(ENA, 150);   // Mengatur kecepatan Motor A (PWM = 150)
  // analogWrite(ENB, 150);   // Mengatur kecepatan Motor B (PWM = 150)
  // delay(300);  // Belok kanan selama 1 detik

  // berhenti
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);   
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
  analogWrite(ENA, 0);   // Mengatur kecepatan Motor A (PWM = 150)
  analogWrite(ENB, 0);   // Mengatur kecepatan Motor B (PWM = 150)
  delay(4000);  // 
 

    // PUTER BALIK KANAN
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   // Motor kiri maju
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Motor kanan mundur
  analogWrite(ENA, 180);   // Mengatur kecepatan Motor A (PWM = 150)
  analogWrite(ENB, 120);   // Mengatur kecepatan Motor B (PWM = 150)
  delay(400);  // Belok kanan selama 1 detik



    // berhenti
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);   
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  
  analogWrite(ENA, 0);   // Mengatur kecepatan Motor A (PWM = 150)
  analogWrite(ENB, 0);   // Mengatur kecepatan Motor B (PWM = 150)
  delay(4000);  // 

  //     // PUTER BALIK Kiri (kurang bagus)
  // digitalWrite(IN1, 0);
  // digitalWrite(IN2, 1);   // Motor kiri maju
  // digitalWrite(IN3, 1);
  // digitalWrite(IN4, 0);  // Motor kanan mundur
  // analogWrite(ENA, 130);   // Mengatur kecepatan Motor A (PWM = 150)
  // analogWrite(ENB, 180);   // Mengatur kecepatan Motor B (PWM = 150)
  // delay(600);  // Belok kanan selama 1 detik

  //     // PUter balik kanan mundut 
  // digitalWrite(IN1, 0);
  // digitalWrite(IN2, LOW);   // Motor kiri maju
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);  // Motor kanan mundur
  // analogWrite(ENA, 80);   // Mengatur kecepatan Motor A (PWM = 150)
  // analogWrite(ENB, 120);   // Mengatur kecepatan Motor B (PWM = 150)
  // delay(1000);  // Belok kanan selama 1 detik


}
