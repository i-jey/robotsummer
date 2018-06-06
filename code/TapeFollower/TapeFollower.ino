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
  while(startbutton()) 
  {
    LCD.setCursor(0,0);
    LCD.print("Press start");
  }

  //Start countdown
  LCD.setCursor(0,0);
  LCD.print("Starting in");
  for(int i = 3; i>=1; i--) {
    LCD.setCursor(0,1);
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
  if(stopbutton()) {
    motor.stop_all();
    return;
  }
  else {
  //PID:

    Kp = knob(6);
    Ki = knob(7)/100;
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Kp: ");
    LCD.print(Kp);
    LCD.setCursor(0, 1);
    LCD.print("Ki: ");
    LCD.print(Ki);

    //Read both QRDs
    currentLeftReading = analogRead(leftQRDPin);
    currentRightReading = analogRead(rightQRDPin);

    //Error = amount off tape, compared to start
    currentLeftError = currentLeftReading - startingLeftReading;
    currentRightError = currentRightReading - startingRightReading;

    //output = Kp*error + Ki*(sum of error * interval) + Kd*(change in error / interval)
    leftMotorIncrease = Kp*(currentLeftError) + Ki*(leftErrorSum) + Kd*(currentLeftError - lastLeftError);
    rightMotorIncrease = Kp*(currentRightError) + Ki*(rightErrorSum) + Kd*(currentRightError - lastRightError);
  
    //Increase motor speeds accordingly. If still on the tape, motorIncrease should be 0, and that motor will go back to default speed
    motor.speed(leftMotor, defaultMotorSpeed + leftMotorIncrease);
    motor.speed(rightMotor, defaultMotorSpeed + rightMotorIncrease);
  
    //Increment error integrals
    leftErrorSum += currentLeftError;
    rightErrorSum += currentRightError;
  
    //Store current error for derivative term
    lastLeftError = currentLeftError;
    lastRightError = currentLeftError;
  }
  /*
  // read left and right
  //if left > right //if sensor is on black, reading is lower
  //speed up left motor
  //same thing for right
  if(analogRead(leftQRDPin) > analogRead(rightQRDPin)) {
    motor.speed(leftMotor, defaultMotorSpeed + speedIncrease);
  }
  else if(analogRead(leftQRDPin) < analogRead(rightQRDPin)) {
    motor.speed(rightMotor, defaultMotorSpeed + speedIncrease);
  }
  else if(analogRead(leftQRDPin) == analogRead(rightQRDPin)) {
    motor.speed(leftMotor, defaultMotorSpeed);
    motor.speed(rightMotor, defaultMotorSpeed);
  }
  */
}
