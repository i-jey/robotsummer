#include "includes.h"

Arm::Arm(){}; // default constructor otherwise C++ whines
Arm::Arm(int clampPin, int armPin, int triggerPin, int closeAngle, int openAngle, int openAngleInside, int lowerAngle, int raiseAngle, int verticalAngle, int phoenixAngle) { 
    this->clampPin = clampPin; 
    this->armPin = armPin; 
    this->triggerPin = triggerPin; 
    this->closeAngle = closeAngle; 
    this->openAngle = openAngle; 
    this->openAngleInside = openAngleInside;
    this->lowerAngle = lowerAngle; 
    this->raiseAngle = raiseAngle; 
    this->verticalAngle = verticalAngle; 
    this->phoenixAngle = phoenixAngle; 
}

void Arm::begin() { 
    clampServo.attach(clampPin); 
    armServo.attach(armPin); 
    
    pinMode(triggerPin, INPUT_PULLDOWN); 
}

bool Arm::ewokDetected() { 
    if (!digitalRead(triggerPin)) { 
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

void Arm::verticalRaise() { 
    armServo.write(verticalAngle); 
}

void Arm::lower() { 
    armServo.write(lowerAngle); 
}

void Arm::customAngle(int angle) { 
    armServo.write(angle); 
}

void Arm::phoenix() { 
    armServo.write(phoenixAngle); 
}