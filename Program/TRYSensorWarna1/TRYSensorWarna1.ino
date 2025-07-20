#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pin I2C custom
#define SDA_PIN A4
#define SCL_PIN A5

// Sensor TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_50MS,
  TCS34725_GAIN_4X
);

void setup() {
  Serial.begin(115200);
  Serial.println("TCS34725 Color Sensor Test");

  Wire.begin(SDA_PIN, SCL_PIN); // inisialisasi I2C di pin 26/25

  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 ditemukan!");
  } else {
    Serial.println("Tidak ditemukan sensor, cek koneksi!");
    while (1);
  }
}

void loop() {
  float red, green, blue;

  tcs.setInterrupt(false); // Nyalakan LED internal sensor
  delay(60);               // Tunggu sensor stabil
  tcs.getRGB(&red, &green, &blue);
  tcs.setInterrupt(true);  // Matikan LED internal sensor

  Serial.print("R: "); Serial.print((int)red);
  Serial.print("\tG: "); Serial.print((int)green);
  Serial.print("\tB: "); Serial.println((int)blue);

  delay(500);
}
