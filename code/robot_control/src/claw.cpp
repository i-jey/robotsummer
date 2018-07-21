#include "includes.h"
#include "claw.h" 

Arm::Arm(){}; // default constructor otherwise C++ whines
Arm::Arm(int clampPin, int armPin, int pushButton, int closeAngle, int openAngle, int lowerAngle, int raiseAngle) { 
    clampServo.attach(clampPin); 
    armServo.attach(armPin);  
    this->pushButton = pushButton; 
    this->closeAngle = closeAngle; 
    this->openAngle = openAngle; 
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

void Arm::open() { 
    clampServo.write(openAngle);     
}

void Arm::raise() { 
    armServo.write(raiseAngle);
    Serial.println("RAISE"); 
}

void Arm::lower() { 
    // clampServo.write(140); 
    armServo.write(lowerAngle); 
    Serial.println("LOWER");
}
