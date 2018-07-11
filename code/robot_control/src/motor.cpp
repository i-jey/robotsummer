#include "includes.h" 
#include "motor.h" 

Motor::Motor(){}; // Default constructor 

Motor::Motor(int motorPin) { 
    // Important: ensure that the pin used is a PWM capable port. 
    // Refer to https://camo.githubusercontent.com/57179305662a573319afec320712289880a7f134/687474703a2f2f77696b692e73746d33326475696e6f2e636f6d2f696d616765732f612f61652f426c756570696c6c70696e6f75742e676966

    this->motorPin = motorPin;
}

void Motor::forward(int speed) {
    // Accepts positive integers only. 
    // Runs the motors forward (check polarity of wiring). 
    analogWrite(motorPin, speed); 
}

void Motor::reverse(int speed) { 
    // Accepts positive integers only. 
    // Runs the motors in reverse (check polarity of wiring).
    analogWrite(motorPin, speed*-1); 
}