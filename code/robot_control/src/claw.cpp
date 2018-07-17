#include "includes.h"
#include "claw.h" 

Arm::Arm(){}; // default constructor otherwise C++ whines
Arm::Arm(int clampPin, int armPin, int pushButton) { 
    clampServo.attach(clampPin); 
    armServo.attach(armPin);  
    this->pushButton = pushButton; 
}

bool Arm::close() {
    if (digitalRead(pushButton)) { 
        clampServo.write(150);
        return true; 
    } 
    return false; 
}

bool Arm::open() { 
    clampServo.write(50); 
    return true; 
}

bool Arm::raise() { 
    armServo.write(140);
    return true;
}

bool Arm::lower() { 
    armServo.write(0); 
    return true; 
}
