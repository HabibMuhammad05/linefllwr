#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pin connections for sensors
const int sensorPin1 = A7;
const int sensorPin2 = A6;
const int selectorPin1 = 2;
const int selectorPin2 = 13;

// Define pin connections for motor driver
int ENA = 6;
int ENB = 3;
int IN1 = 5;
int IN2 = 4;
int IN3 = 8;
int IN4 = 7;

// Define push button pins
const int startButtonPin = A0;
const int nextPlanButtonPin = A1;
const int prevPlanButtonPin = A2;

// Servo declarations
Servo servo1;
Servo servo2;
int servoPin1 = 10;
int servoPin2 = 11;

// Persimpangan counter
int intersectionCounter = 0;

// Flag untuk start
bool isStartPressed = false;

// Variabel untuk menyimpan plan yang dipilih
int selectedPlan = 0;
const char* plans[] = {"Plan A", "Plan B", "Plan C", "Plan D", "Plan E", "Plan F"};
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
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(220);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to execute a right turn
void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   // Motor kiri maju
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(220);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to execute a U-turn
void uTurnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);   // Motor kiri maju
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  // Motor kanan mundur
    analogWrite(ENA, 180);
    analogWrite(ENB, 120);
    delay(400);  // Kurangi delay untuk respon yang lebih cepat
}

// Function to pick up an object
void pickObject() {
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
                turnLeft();  // Belok kiri pada persimpangan 14
            } else if (intersectionCounter == 15) {
                mundur();      // Mundur selama 100 ms sebelum meletakkan objek
                placeObject(); // Letakan objek pada persimpangan 15
                uTurnRight();  // Putar balik setelah meletakan objek
            } else if (intersectionCounter == 16) {
                turnLeft();  // Belok kiri pada persimpangan 16
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
    }
}


void setup() {
    Serial.begin(9600);

    servo1.attach(servoPin1);
    servo2.attach(servoPin2);

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
        if (sensorValues[1] > 150 && sensorValues[0] > 150 &&
            sensorValues[5] > 150 && sensorValues[6] > 150) {

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
            int speedKiri = 65;
            int speedKanan = 65;

            // Optimasi respons kecepatan
            if (sensorValues[1] > 200 && sensorValues[0] > 200) {
                speedKiri = 60;
                speedKanan = 70;
            } else if (sensorValues[5] > 200 && sensorValues[6] > 200) {
                speedKiri = 70;
                speedKanan = 60;
            } else if (sensorValues[1] > 200) {
                speedKiri = 50;
                speedKanan = 80;
            } else if (sensorValues[3] > 200) {
                speedKiri = 40;
                speedKanan = 90;
            } else if (sensorValues[2] > 200) {
                speedKiri = 0;
                speedKanan = 140;
            } else if (sensorValues[0] > 200) {
                speedKiri = 62;
                speedKanan = 62;
            } else if (sensorValues[5] > 200) {
                speedKiri = 62;
                speedKanan = 62;
            } else if (sensorValues[6] > 200) {
                speedKiri = 80;
                speedKanan = 50;
            } else if (sensorValues[4] > 200) {
                speedKiri = 90;
                speedKanan = 40;
            } else if (sensorValues[7] > 200) {
                speedKiri = 140;
                speedKanan = 0;
            } else if (!sensorAboveThreshold) {
                speedKiri = 0;
                speedKanan = 0;
            } else {
                speedKiri = 65;
                speedKanan = 65;
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
