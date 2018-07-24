#include "includes.h"

Arm::Arm(){}; // default constructor otherwise C++ whines
Arm::Arm(int clampPin, int armPin, int pushButton, int closeAngle, int openAngle, int openAngleInside, int lowerAngle, int raiseAngle) { 
    clampServo.attach(clampPin); 
    armServo.attach(armPin);  
    this->pushButton = pushButton; 
    this->closeAngle = closeAngle; 
    this->openAngle = openAngle; 
    this->openAngleInside = openAngleInside;
    this->lowerAngle = lowerAngle; 
    this->raiseAngle = raiseAngle; 
}

bool Arm::ewokDetected() { 
    if (digitalRead(pushButton)) { 
        return true; 
    }
    return false; 
}

void Arm::close() {
    clampServo.write(closeAngle);
}

void Arm::open(bool isInside) { 
    if (isInside) {
        clampServo.write(openAngleInside);
    }
    else { 
        clampServo.write(openAngle);  
    }
}

void Arm::raise() { 
    armServo.write(raiseAngle);
}

void Arm::lower() { 
    armServo.write(lowerAngle); 
}
