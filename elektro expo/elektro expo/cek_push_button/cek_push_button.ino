// Pin untuk push button
const int buttonPinA0 = A0;
const int buttonPinA1 = A1;
const int buttonPinA2 = A2;
const int buttonPinA3 = A3;

// Variabel untuk menyimpan status tombol
int buttonStateA0 = 0;
int buttonStateA1 = 0;
int buttonStateA2 = 0;
int buttonStateA3 = 0;

void setup() {
  // Inisialisasi serial komunikasi
  Serial.begin(9600);
  
  // Set pin sebagai input
  pinMode(buttonPinA0, INPUT);
  pinMode(buttonPinA1, INPUT);
  pinMode(buttonPinA2, INPUT);
  pinMode(buttonPinA3, INPUT);
}

void loop() {
  // Membaca status push button
  buttonStateA0 = digitalRead(buttonPinA0);
  buttonStateA1 = digitalRead(buttonPinA1);
  buttonStateA2 = digitalRead(buttonPinA2);
  buttonStateA3 = digitalRead(buttonPinA3);
  
  // Menampilkan status push button di Serial Monitor
  Serial.print("A0: ");
  Serial.print(buttonStateA0);
  Serial.print(" | A1: ");
  Serial.print(buttonStateA1);
  Serial.print(" | A2: ");
  Serial.print(buttonStateA2);
  Serial.print(" | A3: ");
  Serial.println(buttonStateA3);
  
  // Tunggu 500 ms sebelum membaca kembali
  delay(100);
}
