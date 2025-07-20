#include <Servo.h>

Servo gripper;  

#define GRIPPER_PIN 5   // gunakan pin PWM Nano (3,5,6,9,10,11)

 //Derajat posisi gripper
#define GRIPPER_BUKA   90   // sesuaikan derajat buka
#define GRIPPER_TUTUP   0   // sesuaikan derajat tutup

void setup() {
gripper.attach(GRIPPER_PIN);
gripper.write(GRIPPER_BUKA);  // posisi awal buka
}

void loop() {
 //Tutup gripper
gripper.write(GRIPPER_TUTUP);
delay(3000);  // tunggu 3 detik

 //Buka gripper
gripper.write(GRIPPER_BUKA);
 delay(3000);  // tunggu 3 detik
}

//#include <Servo.h>

//Servo lengan;

//#define LENGAN_PIN 3  // gunakan pin PWM Nano (3,5,6,9,10,11)

// Derajat posisi lengan
//#define LENGAN_BAWAH  45   // sesuaikan untuk posisi bawah
//#define LENGAN_ATAS  150   // sesuaikan untuk posisi atas

//void setup() {
  //lengan.attach(LENGAN_PIN);
  //lengan.write(LENGAN_BAWAH);  // posisi awal di bawah
//}

//void loop() {
  // Naikkan lengan
  //lengan.write(LENGAN_ATAS);
  //delay(3000);  // tahan 3 detik

  // Turunkan lengan
  //lengan.write(LENGAN_BAWAH);
  //delay(3000);  // tahan 3 detik
//}
