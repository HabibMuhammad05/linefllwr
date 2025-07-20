// === Konfigurasi Pin ===
#define SEL_A A0   // pin selektor A (LSB)
#define SEL_B A1   // pin selektor B (MSB)
#define X_PIN A2  // output multiplexer X (S1-S4)
#define Y_PIN A3  // output multiplexer Y (S5-S8)

// Array untuk menyimpan data sensor
int sensorValues[8];

// Urutan cetak sesuai posisi sensor fisik dari kiri ke kanan
// Hasil pengecekan kamu: S6, S8, S7, S5, S3, S2, S1, S4
int sensorOrder[8] = {5, 7, 6, 4, 2, 1, 0, 3};

void setup() {
  Serial.begin(9600);

  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  // Pastikan awal selektor 0
  digitalWrite(SEL_A, LOW);
  digitalWrite(SEL_B, LOW);
}

void loop() {
  // Baca semua sensor
  readAllSensors();

  // Cetak hasil pembacaan sesuai urutan kiri → kanan
  for (int i = 0; i < 8; i++) {
    int idx = sensorOrder[i];       // ambil index sesuai mapping
    Serial.print("S");
    Serial.print(i + 1);            // tampilkan S1..S8 (urut kiri→kanan)
    Serial.print(": ");
    Serial.print(sensorValues[idx]); // ambil data dari index asli
    Serial.print("\t");
  }
  Serial.println();

  delay(100); // jeda supaya serial terbaca rapi
}

// === Fungsi untuk membaca semua sensor ===
void readAllSensors() {
  for (int i = 0; i < 4; i++) {
    // Set selektor
    digitalWrite(SEL_A, i & 0x01);        // bit 0 → A
    digitalWrite(SEL_B, (i >> 1) & 0x01); // bit 1 → B

    delayMicroseconds(5); // kecil saja untuk settling

    // Simpan hasil sesuai channel default
    sensorValues[i]     = analogRead(X_PIN); // S1..S4
    sensorValues[i + 4] = analogRead(Y_PIN); // S5..S8
  }
}
