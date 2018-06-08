#include <phys253.h>
#include <LiquidCrystal.h>

//******* Pins *******//
const int leftMotor = 0;
const int rightMotor = 1;
const int leftQRDPin = 0;
const int rightQRDPin = 5;

//******* Constants *******//
const int defaultMotorSpeed = 200;
int Kp = 1;
int Ki = 1;
int Kd = 1;
//const int speedIncrease = 20;
//const int sensorTolerance = 5;

//******* PID variables *******//
int startingLeftReading;
int startingRightReading;
int currentLeftReading;
int currentRightReading;

int currentLeftError;
int currentRightError;
int lastLeftError = 0;
int lastRightError = 0;

int leftErrorSum = 0;
int rightErrorSum = 0;

int leftMotorIncrease;
int rightMotorIncrease;

void setup() {
#include <phys253setup.txt>
  Serial.begin(9600);

  //Wait for start button to be pushed
  while (startbutton())
  {
    LCD.setCursor(0, 0);
    LCD.print("Press start");
  }

  //Start countdown
  LCD.setCursor(0, 0);
  LCD.print("Starting in");
  for (int i = 3; i >= 1; i--) {
    LCD.setCursor(0, 1);
    LCD.print(i);
    delay(1000);
  }

  //Get initial reading of tape for error comparison
  startingLeftReading = analogRead(leftQRDPin);
  startingRightReading = analogRead(rightQRDPin);

  //Start at default speed
  motor.speed(leftMotor, defaultMotorSpeed);
  motor.speed(rightMotor, defaultMotorSpeed);
}

void loop() {
  unsigned long leftQRD = analogRead(leftQRDPin); 
  unsigned long rightQRD = analogRead(rightQRDPin); 
  unsigned long dx = 1.0; 
  
  // Turn right
  if (leftQRD - rightQRD > dx) { 
    motor.speed(rightMotor, defaultMotorSpeed + 50); 
  }
  // Turn left
  else if (rightQRD - leftQRD > dx) { 
    motor.speed(leftMotor, defaultMotorSpeed + 50); 
  }
  else { 
    motor.speed(rightMotor, defaultMotorSpeed);
    motor.speed(leftMotor, defaultMotorSpeed); 
  }
}
