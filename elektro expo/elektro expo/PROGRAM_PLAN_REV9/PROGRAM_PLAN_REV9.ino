#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pin connections for sensors
const int sensorPin1 = A3;
const int sensorPin2 = A2;
const int selectorPin1 = A1;
const int selectorPin2 = A0;

// Define pin connections for motor driver
int ENA = 10;
int ENB = 11;
int IN2 = 6;
int IN1 = 7;
int IN4 = 8;
int IN3 = 9;

// Define push button pins
const int startButtonPin = A4;
const int nextPlanButtonPin = A5;
const int prevPlanButtonPin = A6;
const int menuButtonPin = A7;

// Servo declarations
Servo servo1;
Servo servo2;
int servoPin1 = 5;
int servoPin2 = 3;

// Persimpangan counter
int intersectionCounter = 0;
int startingIntersection = 1;

// Flag untuk start
bool isStartPressed = false;
bool inSettingMenu = false;

// Variabel untuk menyimpan plan yang dipilih
int selectedPlan = 0;
const char* plans[] = {"Plan A", "Plan B", "Plan C", "Plan D", "Plan E", "Plan F", "Plan G", "Plan H"};
const int numPlans = sizeof(plans) / sizeof(plans[0]);

// Function to read sensors
int readSensor(int selectorValue1, int selectorValue2, int sensorPin) {
    digitalWrite(selectorPin1, selectorValue1);
    digitalWrite(selectorPin2, selectorValue2);
    return analogRead(sensorPin);
}

// Function to execute a left turn
void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);  // Motor kiri mundur
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);   // Motor kanan maju
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
    delay(600);  // Kurangi delay untuk respon yang lebih cepat
}

//fungsi maju selama 500 mili detik 
void maju(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("maju !!! ");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);  
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);   
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    delay(400);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to execute a right turn
void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   // Motor kiri maju
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
    delay(350);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to execute a U-turn
void uTurnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   // Motor kiri maju
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 200);
    analogWrite(ENB, 140);
    delay(530);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to pick up an object
void pickObject() {
    servo1.attach(servoPin1);
    servo2.attach(servoPin2);
    servo1.write(140);
    servo2.write(100);
    delay(1000);
    servo1.write(50);
    delay(500);
    servo2.write(25);
    delay(500);
    servo1.write(140);
}

// Function to place an object
void placeObject() {
    servo1.write(140);
    servo2.write(25);
    delay(1000);
    servo1.write(50);
    delay(1000);
    servo2.write(100);
    delay(500);
    servo1.write(140);
}

void mundur(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reverse ");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);  // Motor kiri mundur
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(100);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

// Function to change the selected plan
void changePlan(int direction) {
    selectedPlan = (selectedPlan + direction + numPlans) % numPlans;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(plans[selectedPlan]);
}

// Function to change the starting intersection
void changeStartingIntersection(int direction) {
    startingIntersection = max(1, startingIntersection + direction);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start at Int:");
    lcd.setCursor(0, 1);
    lcd.print(startingIntersection);
}

// Function to execute the selected plan
void executePlan() {
    // Tampilkan persimpangan yang dicapai di LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Persimpangan ke-");
    lcd.setCursor(0, 1);
    lcd.print(intersectionCounter);

 switch (selectedPlan) {
        case 0:  // Plan A
            if (intersectionCounter == 1) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 2) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 2
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 3) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 4) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 4
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 5) {
                turnRight();  // Belok kanan pada persimpangan 5
            } else if (intersectionCounter == 6) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 7) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 2
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 8) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 9) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 4
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 10) {
                turnRight();  // Belok kanan pada persimpangan 5
            } else if (intersectionCounter == 11) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 12) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 2
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 13) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 14) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 4
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 15) {
                turnRight();  // Belok kanan pada persimpangan 5
            }
            break;

        case 1:  // Plan B
            if (intersectionCounter == 1) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 2) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 2
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 3) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 4) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 4
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 5) {
                turnRight();  // Belok kanan pada persimpangan 5
            } else if (intersectionCounter == 6) {
                turnLeft();  // Belok kiri pada persimpangan 6
            } else if (intersectionCounter == 7) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil Y6objek pada persimpangan 7
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 8) {
                turnLeft();  // Belok kiri pada persimpangan 8
            } else if (intersectionCounter == 9) {
                turnRight();  // Belok kanan pada persimpangan 9
            } else if (intersectionCounter == 10) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 10
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 11) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 12) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 12
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 13) {
                turnRight();  // Belok kanan pada persimpangan 13
            } else if (intersectionCounter == 14) {
                turnLeft();  // Belok kiri pada persimpangan 14
            } else if (intersectionCounter == 15) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 15
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 16) {
                turnRight();  // Belok kanan pada persimpangan 16
            } else if (intersectionCounter == 17) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 18) {
                // Pada persimpangan terakhir, robot berhenti
                analogWrite(ENA, 0);
                analogWrite(ENB, 0);
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                lcd.setCursor(0, 0);
                lcd.print("Robot Stopped");
            }
            break;

        case 2:  // Plan C
            if (intersectionCounter == 1) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 2) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 2
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 3) {
                turnLeft();  // Belok kiri pada persimpangan 3
            } else if (intersectionCounter == 4) {
                turnRight();  // Belok kanan pada persimpangan 4
            } else if (intersectionCounter == 5) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 5
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 6) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 7) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 7
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 8) {
                turnLeft();  // Belok kiri pada persimpangan 8
            } else if (intersectionCounter == 9) {
                turnRight();  // Belok kanan pada persimpangan 9
            } else if (intersectionCounter == 10) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 10
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 11) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 12) {
                mundur();     // Mundur selama 100 ms sebelum mengambil objek
                pickObject();  // Ambil objek pada persimpangan 12
                uTurnRight();  // Putar balik setelah mengambil objek
            } else if (intersectionCounter == 13) {
                turnRight();  // Belok kanan pada persimpangan 13
            } else if (intersectionCounter == 14) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 15) {
                turnLeft();  // Belok kiri pada persimpangan 15
            } else if (intersectionCounter == 16) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 16
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 17) {
                turnRight();  // Belok kanan pada persimpangan 17
            } else if (intersectionCounter == 18) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 19) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 20) {
                // Pada persimpangan terakhir, robot berhenti
                analogWrite(ENA, 0);
                analogWrite(ENB, 0);
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                lcd.setCursor(0, 0);
                lcd.print("Robot Stopped");
            }
            break;

        case 3:  // Plan D
            if (intersectionCounter == 1) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 2) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 3) {
                turnLeft();
            } else if (intersectionCounter == 4) {
                turnRight();
            } else if (intersectionCounter == 5) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 6) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 7) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 8) {
                turnRight();
            } else if (intersectionCounter == 9) {
                turnLeft();
            } else if (intersectionCounter == 10) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 11) {
                turnRight();
            } else if (intersectionCounter == 12) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 13) {
                turnLeft();
            } else if (intersectionCounter == 14) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 15) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 16) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 17) {
                turnRight();
            } else if (intersectionCounter == 18) {
                // Pada persimpangan terakhir, robot berhenti
                analogWrite(ENA, 0);
                analogWrite(ENB, 0);
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                lcd.setCursor(0, 0);
                lcd.print("Robot Stopped");
            }
            break;

        case 4:  // Plan E
            if (intersectionCounter == 1) {
                turnLeft();
            } else if (intersectionCounter == 2) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 3) {
                turnLeft();
            } else if (intersectionCounter == 4) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 5) {
                turnRight();
            } else if (intersectionCounter == 6) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 7) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 8) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 9) {
                turnRight();
            } else if (intersectionCounter == 10) {
                turnLeft();
            } else if (intersectionCounter == 11) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 12) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 13) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 14) {
                turnRight();
            } else if (intersectionCounter == 15) {
                turnLeft();
            } else if (intersectionCounter == 16) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 17) {
                turnRight();
            } else if (intersectionCounter == 18) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 19) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 20) {
                // Pada persimpangan terakhir, robot berhenti
                analogWrite(ENA, 0);
                analogWrite(ENB, 0);
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                lcd.setCursor(0, 0);
                lcd.print("Robot Stopped");
            }
            break;

        case 5:  // Plan F
            if (intersectionCounter == 1) {
                turnLeft();
            } else if (intersectionCounter == 2) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 3) {
                turnLeft();
            } else if (intersectionCounter == 4) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 5) {
                turnRight();
            } else if (intersectionCounter == 6) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 7) {
                turnLeft();
            } else if (intersectionCounter == 8) {
                turnRight();
            } else if (intersectionCounter == 9) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 10) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 11) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 12) {
                turnRight();
            } else if (intersectionCounter == 13) {
                turnLeft();
            } else if (intersectionCounter == 14) {
                mundur();
                pickObject();
                uTurnRight();
            } else if (intersectionCounter == 15) {
                turnRight();
            } else if (intersectionCounter == 16) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 17) {
                turnLeft();
            } else if (intersectionCounter == 18) {
                mundur();
                placeObject();
                uTurnRight();
            } else if (intersectionCounter == 19) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 20) {
                // Abaikan dan tetap mengikuti garis
            } else if (intersectionCounter == 21) {
                // Pada persimpangan terakhir, robot berhenti
                analogWrite(ENA, 0);
                analogWrite(ENB, 0);
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                lcd.setCursor(0, 0);
                lcd.print("Robot Stopped");
            }
            break;

        case 6:  // Plan G
            if (intersectionCounter == 1) {
                turnLeft();  // Belok kiri pada persimpangan 1
            } else if (intersectionCounter == 2) {
                turnRight();  // Belok kiri pada persimpangan 2
            } else if (intersectionCounter == 3) {
                turnRight();  // Belok kiri pada persimpangan 3
            } else if (intersectionCounter == 4) {
                turnLeft();  // Belok kiri pada persimpangan 3   
            }
            break;

        case 7:  // Plan H
            if (intersectionCounter == 1) {
                turnRight();  // Belok kanan pada persimpangan 1
            } else if (intersectionCounter == 2) {
                turnLeft();  // Belok kanan pada persimpangan 2
            } else if (intersectionCounter == 3) {
                turnRight();  // Belok kanan pada persimpangan 3
                
            }
            break;
    }
}

void setup() {
    Serial.begin(9600);

    // servo1.attach(servoPin1);
    // servo2.attach(servoPin2);

    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Set selector and motor pins as outputs
    pinMode(selectorPin1, OUTPUT);
    pinMode(selectorPin2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Set push button pins as input
    pinMode(startButtonPin, INPUT_PULLUP);
    pinMode(nextPlanButtonPin, INPUT_PULLUP);
    pinMode(prevPlanButtonPin, INPUT_PULLUP);
    pinMode(menuButtonPin, INPUT_PULLUP);

    // Initialize motors to stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    // Display the initial plan on the LCD
    lcd.setCursor(0, 0);
    lcd.print(plans[selectedPlan]);
}

void loop() {
    // Jika berada di menu pengaturan
    if (inSettingMenu) {
        if (digitalRead(nextPlanButtonPin) == HIGH) {
            changeStartingIntersection(1);  // Menambah nilai persimpangan awal
            delay(300);  // Debounce delay
        }
        if (digitalRead(prevPlanButtonPin) == HIGH) {
            changeStartingIntersection(-1);  // Mengurangi nilai persimpangan awal
            delay(300);  // Debounce delay
        }
        if (digitalRead(startButtonPin) == HIGH) {
            inSettingMenu = false;  // Keluar dari menu pengaturan
            intersectionCounter = startingIntersection;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Startiiing");
            delay(1000);  // Menunggu 1 detik
            lcd.clear();

            // Tambahkan logika untuk membuat robot maju
            maju();  // Fungsi untuk bergerak maju
            delay(100);  // Durasi bergerak maju selama 100 ms

            isStartPressed = true;  // Menandakan bahwa tombol start telah ditekan
        }
    } else {
        // Menunggu tombol start ditekan
        if (!isStartPressed) {
            if (digitalRead(nextPlanButtonPin) == HIGH) {
                changePlan(1);  // Pindah ke plan berikutnya
                delay(300);     // Debounce delay
            }
            if (digitalRead(prevPlanButtonPin) == HIGH) {
                changePlan(-1);  // Pindah ke plan sebelumnya
                delay(300);      // Debounce delay
            }
            if (digitalRead(startButtonPin) == HIGH) {
                isStartPressed = true;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Starting...");
                delay(1000);
                lcd.clear();
                maju();
                delay(100);
            }
            if (digitalRead(menuButtonPin) == HIGH) {
                inSettingMenu = true;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Setting Mode");
                delay(1000);
                changeStartingIntersection(0);  // Tampilkan persimpangan awal
            }
        }

        if (isStartPressed) {
// Logika mengikuti garis
        int sensorValues[8];
        bool sensorAboveThreshold = false;

        for (int i = 0; i < 8; i++) {
            int selectorValue1 = (i & 0b10) >> 1;
            int selectorValue2 = i & 0b01;

            if (i < 4) {
                sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin1);
            } else {
                sensorValues[i] = readSensor(selectorValue1, selectorValue2, sensorPin2);
            }

            if (sensorValues[i] > 150) {
                sensorAboveThreshold = true;
            }
        }

        // Cek apakah persimpangan terdeteksi
        if (sensorValues[1] > 200 && sensorValues[0] > 200 &&
            sensorValues[5] > 200 && sensorValues[6] > 200) {

            lcd.setCursor(0, 0);
            lcd.print("Intersection");
            analogWrite(ENA, 0);
            analogWrite(ENB, 0);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            delay(300);  // Kurangi delay untuk respons yang lebih cepat

            intersectionCounter++;
            
            // Eksekusi plan sesuai dengan persimpangan yang terdeteksi
            executePlan();

        } else {
            // Logika mengikuti garis
            int speedKiri = 85;
            int speedKanan = 85;

            // Print sensor values on Serial Monitor for debugging ( cadangan bila pembacaan sensor error maka ini perlu di aktifkan )
             for (int i = 0; i < 8; i++) {
               Serial.print("Sensor ");
               Serial.print(i);
               Serial.print(": ");
               Serial.print(sensorValues[i]);
               Serial.print("\t"); // Tab space for better readability
             }
             Serial.println(); // Move to the next line

            // Optimasi respons kecepatan
            if (sensorValues[1] > 200 && sensorValues[0] > 200) {
                speedKiri = 90;
                speedKanan = 80;
            } else if (sensorValues[5] > 200 && sensorValues[6] > 200) {
                speedKiri = 100;
                speedKanan = 80;
            } else if (sensorValues[1] > 200) {
                speedKiri = 70;
                speedKanan = 140;
            } else if (sensorValues[3] > 200) {
                speedKiri = 60;
                speedKanan = 140;
            } else if (sensorValues[2] > 200) {
                speedKiri = 0;
                speedKanan = 160;
            } else if (sensorValues[0] > 200) {
                speedKiri = 80;
                speedKanan = 85;
            } else if (sensorValues[5] > 200) {
                speedKiri = 80;
                speedKanan = 83;
            } else if (sensorValues[6] > 200) {
                speedKiri = 100;
                speedKanan = 75;
            } else if (sensorValues[4] > 200) {
                speedKiri = 90;
                speedKanan = 60;
            } else if (sensorValues[7] > 200) {
                speedKiri = 140;
                speedKanan = 0;
            } else if (!sensorAboveThreshold) {
                speedKiri = 0;
                speedKanan = 0;
            } else {
                speedKiri = 85;
                speedKanan = 85;
            }

            analogWrite(ENA, speedKiri);
            analogWrite(ENB, speedKanan);

            // Set motor directions (both forward)
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);

            delay(2);  // Kurangi delay untuk respons yang lebih cepat
        }
        }
    }
}
