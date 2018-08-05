#include "includes.h"  

Motor::Motor(){}; // Default constructor 

Motor::Motor(int motorPin1, int motorPin2) { 
    // Important: ensure that the pin used is a PWM capable port. 
    // Refer to https://camo.githubusercontent.com/57179305662a573319afec320712289880a7f134/687474703a2f2f77696b692e73746d33326475696e6f2e636f6d2f696d616765732f612f61652f426c756570696c6c70696e6f75742e676966

    this->motorPin1 = motorPin1;
    this->motorPin2 = motorPin2; 
}

void Motor::begin() { 
    analogWrite(0, motorPin1); 
    pinMode(motorPin1, OUTPUT);
    
    delay(50); 

    analogWrite(0, motorPin2); 
    pinMode(motorPin2, OUTPUT); 
}

void Motor::forward(int speed) {
    // Accepts positive integers between 0-255. 
    // Runs the motors forward (check polarity of wiring).
    analogWrite(motorPin1, speed); 
    analogWrite(motorPin2, 0); 
}

void Motor::reverse(int speed) { 
    // Accepts positive integers only between 0-255. 
    // Runs the motors in reverse (check polarity of wiring).
    analogWrite(motorPin1, 0); 
    analogWrite(motorPin2, speed); 
}

void Motor::write(int speed) { 
    if (speed > 255) {speed = 255;}
    else if (speed < -255) {speed = -255;}
    
    if (speed >= 0) {forward(speed);}
    else if (speed < 0) {reverse(abs(speed));}
}