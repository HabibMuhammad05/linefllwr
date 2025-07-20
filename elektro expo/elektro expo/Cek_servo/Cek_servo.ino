#include <Servo.h>  // Mengimpor library Servo

Servo servo1;  // Membuat objek Servo untuk servo pertama
Servo servo2;  // Membuat objek Servo untuk servo kedua

void setup() {
  // Menghubungkan servo1 ke pin 9 dan servo2 ke pin 10
  servo1.attach(10);
  servo2.attach(11);
}

void loop() {
  // Looping untuk servo1 
  for (int pos = 50; pos <= 140; pos += 1) {
    servo1.write(pos);   // Mengatur posisi servo1
    delay(20);           // Menunggu 20ms untuk kelancaran gerakan
  }
  delay(2000);           // Jeda 2 detik

  // Looping untuk servo1 
  for (int pos = 140; pos >= 50; pos -= 1) {
    servo1.write(pos);   // Mengatur posisi servo1
    delay(20);           // Menunggu 20ms untuk kelancaran gerakan
  }
  delay(2000);           // Jeda 2 detik

  // Looping untuk servo2 
  for (int pos = 25; pos <= 100; pos += 1) {
    servo2.write(pos);   // Mengatur posisi servo2
    delay(20);           // Menunggu 20ms untuk kelancaran gerakan
  }
  delay(2000);           // Jeda 5 detik

  // Looping untuk servo2 
  for (int pos = 100; pos >= 25; pos -= 1) {
    servo2.write(pos);   // Mengatur posisi servo2
    delay(20);           // Menunggu 20ms untuk kelancaran gerakan
  }
  delay(2000);           // Jeda 5 detik
}
