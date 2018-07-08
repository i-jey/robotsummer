#include "includes.h"
#include "claw.h" 

Arm::Arm(){}; // default constructor otherwise C++ whines
Arm::Arm(int clampPin, int armPin, int pushButton) { 
    clampServo.attach(clampPin); 
    armServo.attach(armPin);  
    pushButton = pushButton; 
}

bool Arm::close() {
    if (digitalRead(pushButton)) { 
        clampServo.write(180);
        return true; 
    } 
    return false; 
}

bool Arm::open() { 
    clampServo.write(0); 
    return true; 
}

bool Arm::raise() { 
    armServo.write(180);
    return true;
}

bool Arm::lower() { 
    armServo.write(0); 
    return true; 
}
