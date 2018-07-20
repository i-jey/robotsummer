#include <phys253.h>
#include <LiquidCrystal.h>

//******* Pins *******//
const int leftMotor = 0;
const int rightMotor = 1;
const int leftQRDPin = 1;
const int rightQRDPin = 0;

const int Kp_pin = 4;
const int Kd_pin = 5;
//const int Ki_pin = 0;
const int K_pin = 6;
const int threshold_pin = 7;

//******* Constants *******//
//Working values:
//Kp = 1, Kd = 4, K = 55
const int defaultMotorSpeed = 160;
int threshold = 1;

int K = 0;
int Kp = 0;
int Ki = 0;
int Kd = 0;
float p;
float i;
float d;

int iMax;
int deadBand;
int error = 0;
int lastError = 0;
int gain;
int count = 0;

bool leftQRDState;
bool rightQRDState;

volatile bool state = false;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

void setup() {
  // put your setup code here, to run once:
  #include <phys253setup.txt>
  Serial.begin(9600) ;
}

void loop() {
 
  if (startbutton()){
    state = true;
  }
  if (stopbutton()){
    state = false;
  }

  Kp = analogRead(Kp_pin)/10;
  Kd = analogRead(Kd_pin)/10;
  //Ki = analogRead(Ki_pin)/10;
  K = knob(K_pin)/10;
  threshold = knob(threshold_pin);
  
  // put your main code here, to run repeatedly:
  leftQRDState = analogRead(leftQRDPin) > threshold;
  rightQRDState = analogRead(rightQRDPin) > threshold;
  
  // Case 0: Both on
  if (leftQRDState && rightQRDState){
    error = 0;
  }
  // Case 1: Left off, Right on
  else if (rightQRDState){
    error = -1;
  }
  // Case 2: Right on, Left off
  else if (leftQRDState){
    error = 1;
  }
  // Case 3: Both off
  // Case 3.1: Right previously on
  else if (lastError < 0){
    error = -5;
  }
  // Case 3.2: Left previously on
  else if (lastError > 0){
    error = 5;
  }

  p = Kp*error;
  d = Kd*(error-lastError); //
  i = Ki*error + i;
      if (i > iMax) i = iMax;
      if (i < -iMax) i = -iMax;  
  gain = K*(p + i + d);

  if (state) {
    if (gain < 0){
      leftMotorSpeed = defaultMotorSpeed + abs(gain);
      rightMotorSpeed = defaultMotorSpeed - abs(gain);
      motor.speed(leftMotor, leftMotorSpeed);
      motor.speed(rightMotor, rightMotorSpeed);
    }
    else if (gain > 0){
      rightMotorSpeed = defaultMotorSpeed + abs(gain);
      leftMotorSpeed = defaultMotorSpeed - abs(gain);
      motor.speed(rightMotor, rightMotorSpeed);
      motor.speed(leftMotor, leftMotorSpeed);
    }
    else {
      rightMotorSpeed = defaultMotorSpeed;
      leftMotorSpeed = defaultMotorSpeed;
      motor.speed(rightMotor, rightMotorSpeed);
      motor.speed(leftMotor, leftMotorSpeed);  
    }
  }
  else {
    motor.speed(rightMotor, 0);
    motor.speed(leftMotor, 0);
  }

  lastError = error;

  count++;
  if (count > 2000){
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Kp:");
    LCD.print(Kp);
    LCD.print(" Ki:");
    LCD.print(Ki);
    LCD.print(" Kd:");
    LCD.print(Kd);
    LCD.setCursor(0, 1); 
    LCD.print("G: ");
    LCD.print(K);
    LCD.print(" T: ");
    LCD.print(threshold);
 
    Serial.print("error");
    Serial.println(error);
    Serial.print("Gain");
    Serial.println(gain);
    Serial.print("Left:");
    Serial.println(analogRead(leftQRDPin));
    Serial.print("Right:");
    Serial.println(analogRead(rightQRDPin));
    Serial.print("Left motor speed: ");
    Serial.println(leftMotorSpeed);
    Serial.print("Right motor speed: ");
    Serial.println(rightMotorSpeed);
    
    count = 0;
  }
  //delay(200);
}
