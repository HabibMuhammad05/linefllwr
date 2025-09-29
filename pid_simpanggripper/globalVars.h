// ----------- MUX & Sensor Setup ----------
#define SEL_A A0
#define SEL_B A1
#define X_PIN A2
#define Y_PIN A3

int sensorValues[8];
int sensorOrder[8] = {5, 7, 6, 4, 2, 1, 0, 3}; // Your given order

// ----------- Motor Driver Setup ----------
#define ENA 10   // PWM Left Motor
#define IN1 6
#define IN2 7
#define ENB 11   // PWM Right Motor
#define IN3 8
#define IN4 9

// ----------- Speed Settings ----------
int baseSpeed = 70;
int minSpeed  = 15;
int maxSpeed  = 150;

// ----------- PID Gains ----------
float Kp = 0.4;
float Ki = 0.0005;
float Kd = 0.04;

// Anti-windup & limits
float integral = 0.0;
float integralMax = 200.0;
float integralMin = -200.0;

// PID state
int lastPosition = 0;
unsigned long lastTime = 0;

int loopDelayMs = 1; // run loop every ~10 ms

// ----------- LED + intersection setup ----------
#define LED_PIN 12
unsigned long ledTimer = 0;
bool ledActive = false;

int intersectionCount = 0;   
bool actionDone = false;     

unsigned long lastIntersectionTime = 0; // NEW cooldown timer
unsigned long intersectionCooldown = 500; // ms, adjust if needed

void ioSetup(){
  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
    
}
