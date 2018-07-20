#include <phys253.h>
#include <LiquidCrystal.h>

//******* Pins *******//
const int leftMotor = 0;
const int rightMotor = 1;
const int offsetLKnob = 6;
const int speedKnob = 7;

const int leftQRDPin = 1;
const int rightQRDPin = 0;


//******* Variables *******//
int const baseSpeed = 200;
int motorSpeed = 0;
int leftMotorOffset = 17;
int rightMotorOffset = 0;
int count = 0;
bool state = false;

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
  // put your main code here, to run repeatedly:
  leftMotorOffset = knob(offsetLKnob)/10;
  motorSpeed = baseSpeed + knob(speedKnob)/10;
  
  // put your main code here, to run repeatedly:
  if (state) {
    motor.speed(rightMotor, motorSpeed + leftMotorOffset);
    motor.speed(leftMotor, motorSpeed);
  }
  else {
    motor.speed(rightMotor, 0);
    motor.speed(leftMotor, 0);    
  }

  count++;
  if (count > 500){
    LCD.clear();
    LCD.setCursor(0, 0); 
    LCD.print("Left: ");
    LCD.print(analogRead(leftQRDPin));
    
    LCD.setCursor(0, 1); 
    LCD.print("Right: ");
    LCD.print(analogRead(leftQRDPin));
    count = 0;

    Serial.print("Speed: ");
    Serial.println(motorSpeed); 
    Serial.print("OffsetLeft: ");
    Serial.println(leftMotorOffset);
  }
}
